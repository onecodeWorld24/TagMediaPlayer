#include "windowhelper.h"

#include <cmath>
#include <winuser.h>

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) (static_cast<int>(static_cast<short>(LOWORD(lp))))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp) (static_cast<int>(static_cast<short>(HIWORD(lp))))
#endif

#ifndef IsMinimized
#define IsMinimized(hwnd) (::IsIconic(hwnd))
#endif

#ifndef IsMaximized
#define IsMaximized(hwnd) (::IsZoomed(hwnd))
#endif

#ifndef RECT_WIDTH
#define RECT_WIDTH(rect) ((rect).right - (rect).left)
#endif

#ifndef RECT_HEIGHT
#define RECT_HEIGHT(rect) ((rect).bottom - (rect).top)
#endif

static WNDPROC g_qtWindowProc = nullptr;

static inline constexpr bool isNonClientMessage(const UINT message)
{
    if (((message >= WM_NCCREATE) && (message <= WM_NCACTIVATE))
        || ((message >= WM_NCMOUSEMOVE) && (message <= WM_NCMBUTTONDBLCLK))
        || ((message >= WM_NCXBUTTONDOWN) && (message <= WM_NCXBUTTONDBLCLK))
#if (WINVER >= _WIN32_WINNT_WIN8)
        || ((message >= WM_NCPOINTERUPDATE) && (message <= WM_NCPOINTERUP))
#endif
        || ((message == WM_NCMOUSEHOVER) || (message == WM_NCMOUSELEAVE))) {
        return true;
    } else {
        return false;
    }
}
static MSG createMessageBlock(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    MSG msg;
    msg.hwnd = hWnd;       // re-create MSG structure
    msg.message = message; // time and pt fields ignored
    msg.wParam = wParam;
    msg.lParam = lParam;

    const DWORD dwScreenPos = ::GetMessagePos();
    msg.pt.x = GET_X_LPARAM(dwScreenPos);
    msg.pt.y = GET_Y_LPARAM(dwScreenPos);
    if (!isNonClientMessage(message)) {
        ::ScreenToClient(hWnd, &msg.pt);
    }
    return msg;
}
static inline constexpr bool isInputMessage(UINT m)
{
    switch (m) {
    case WM_IME_STARTCOMPOSITION:
    case WM_IME_ENDCOMPOSITION:
    case WM_IME_COMPOSITION:
    case WM_INPUT:
    // case WM_TOUCH:
    case WM_MOUSEHOVER:
    case WM_MOUSELEAVE:
    case WM_NCMOUSEHOVER:
    case WM_NCMOUSELEAVE:
    case WM_SIZING:
    case WM_MOVING:
    case WM_SYSCOMMAND:
    case WM_COMMAND:
    // case WM_DWMNCRENDERINGCHANGED:
    case WM_PAINT:
        return true;
    default:
        break;
    }
    return (m >= WM_MOUSEFIRST && m <= WM_MOUSELAST) || (m >= WM_NCMOUSEMOVE && m <= WM_NCXBUTTONDBLCLK)
           || (m >= WM_KEYFIRST && m <= WM_KEYLAST);
}

bool customWindowHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT *result)
{
    switch (message) {
    case WM_NCHITTEST: {
        // [[maybe_unused]] const auto &hitTestRecorder = qScopeGuard([this, result]() {
        //     lastHitTestResult = getHitWindowPart(int(*result)); //
        // });
        POINT nativeGlobalPos{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        POINT nativeLocalPos = nativeGlobalPos;
        ::ScreenToClient(hWnd, &nativeLocalPos);

        RECT clientRect{0, 0, 0, 0};
        ::GetClientRect(hWnd, &clientRect);
        auto clientWidth = RECT_WIDTH(clientRect);
        auto clientHeight = RECT_HEIGHT(clientRect);

        // QPoint qtScenePos = QHighDpi::fromNativeLocalPosition(point2qpoint(nativeLocalPos), m_windowHandle);
        QPoint qtScenePos(nativeLocalPos.x, nativeLocalPos.y);

        bool isFixedSize = false /*m_delegate->isHostSizeFixed(m_host)*/;
        bool isTitleBar = false /*isInTitleBarDraggableArea(qtScenePos)*/;
        bool dontOverrideCursor = false; // ### TODO

        bool max = IsMaximized(hWnd);
        bool full = false /*isFullScreen(hWnd)*/;
        int  frameSize = 10 /*getResizeBorderThickness(hWnd)*/;
        bool isTop = (nativeLocalPos.y < frameSize);

        if (/*isSystemBorderEnabled()*/ true) {
            // This will handle the left, right and bottom parts of the frame
            // because we didn't change them.
            LRESULT originalHitTestResult = ::DefWindowProcW(hWnd, WM_NCHITTEST, 0, lParam);
            if (originalHitTestResult != HTCLIENT) {
                // Even if the window is not resizable, we still can't return HTCLIENT here
                // because when we enter this code path, it means the mouse cursor is
                // outside the window, that is, the three transparent window resize area.
                // Returning HTCLIENT will confuse Windows, we can't put our controls there
                // anyway.
                *result = ((isFixedSize || dontOverrideCursor) ? HTBORDER : originalHitTestResult);
                return true;
            }
            if (full) {
                *result = HTCLIENT;
                return true;
            }
            if (max) {
                *result = (isTitleBar ? HTCAPTION : HTCLIENT);
                return true;
            }
            // At this point, we know that the cursor is inside the client area,
            // so it has to be either the little border at the top of our custom
            // title bar or the drag bar. Apparently, it must be the drag bar or
            // the little border at the top which the user can use to move or
            // resize the window.
            if (isTop) {
                // Return HTCLIENT instead of HTBORDER here, because the mouse is
                // inside our homemade title bar now, return HTCLIENT to let our
                // title bar can still capture mouse events.
                *result = ((isFixedSize || dontOverrideCursor) ? (isTitleBar ? HTCAPTION : HTCLIENT) : HTTOP);
                return true;
            }
            if (isTitleBar) {
                *result = HTCAPTION;
                return true;
            }
            *result = HTCLIENT;
            return true;
        } else {
            if (full) {
                *result = HTCLIENT;
                return true;
            }
            if (max) {
                *result = (isTitleBar ? HTCAPTION : HTCLIENT);
                return true;
            }
            if (!isFixedSize) {
                const bool isBottom = (nativeLocalPos.y >= (clientHeight - frameSize));
                // Make the border a little wider to let the user easy to resize on corners.
                const auto scaleFactor = ((isTop || isBottom) ? qreal(2) : qreal(1));
                const int  scaledFrameSize = std::round(qreal(frameSize) * scaleFactor);
                const bool isLeft = (nativeLocalPos.x < scaledFrameSize);
                const bool isRight = (nativeLocalPos.x >= (clientWidth - scaledFrameSize));
                if (dontOverrideCursor && (isTop || isBottom || isLeft || isRight)) {
                    // Return HTCLIENT instead of HTBORDER here, because the mouse is
                    // inside the window now, return HTCLIENT to let the controls
                    // inside our window can still capture mouse events.
                    *result = (isTitleBar ? HTCAPTION : HTCLIENT);
                    return true;
                }
                if (isTop) {
                    if (isLeft) {
                        *result = HTTOPLEFT;
                        return true;
                    }
                    if (isRight) {
                        *result = HTTOPRIGHT;
                        return true;
                    }
                    *result = HTTOP;
                    return true;
                }
                if (isBottom) {
                    if (isLeft) {
                        *result = HTBOTTOMLEFT;
                        return true;
                    }
                    if (isRight) {
                        *result = HTBOTTOMRIGHT;
                        return true;
                    }
                    *result = HTBOTTOM;
                    return true;
                }
                if (isLeft) {
                    *result = HTLEFT;
                    return true;
                }
                if (isRight) {
                    *result = HTRIGHT;
                    return true;
                }
            }
            if (isTitleBar) {
                *result = HTCAPTION;
                return true;
            }
            *result = HTCLIENT;
            return true;
        }
    }
    case WM_WINDOWPOSCHANGING: {
        // ### FIXME: How does this problem happen and why is it solved?
        // When toggling the "Show theme color in title bar and window border" setting in
        // Windows Settings, or calling `DrawMenuBar()`, Windows sends a message of
        // WM_WINDOWPOSCHANGING with flags 0x37. If we do not process this message,
        // the client area as a whole will shift to the left, which looks very abnormal if
        // we don't repaint it. This exception disappears if we add SWP_NOCOPYBITS flag.
        // But I don't know what caused the problem, or why this would solve it.
        static constexpr const auto kBadWindowPosFlag = SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
                                                        | SWP_FRAMECHANGED;
        const auto windowPos = reinterpret_cast<LPWINDOWPOS>(lParam);
        if (windowPos->flags == kBadWindowPosFlag) {
            windowPos->flags |= SWP_NOCOPYBITS;
        }
        break;
    }

    default:
        break;
    }
}

extern "C" LRESULT QT_WIN_CALLBACK QWKHookedWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Q_ASSERT(hWnd);
    if (!hWnd) {
        return FALSE;
    }

    // Search window context
    // auto ctx = g_wndProcHash->value(hWnd);
    // if (!ctx) {
    //     return ::DefWindowProcW(hWnd, message, wParam, lParam);
    // }

    // WindowsNativeEventFilter::lastMessageContext = ctx;
    // const auto &contextCleaner = qScopeGuard([]() {
    //     WindowsNativeEventFilter::lastMessageContext = nullptr; //
    // });

    // Since Qt does the necessary processing of the WM_NCCALCSIZE message, we
    // need to forward it right away and process it in our native event filter.
    if (message == WM_NCCALCSIZE) {
        return ::CallWindowProcW(g_qtWindowProc, hWnd, message, wParam, lParam);
    }
    // *result = FALSE;
    switch (message) {
    case WM_DESTROY:
    case WM_CLOSE:
    case WM_NCDESTROY:
        // Undocumented messages:
        // case WM_UAHDESTROYWINDOW:
        // case WM_UNREGISTER_WINDOW_SERVICES:
        {
            MSG msg = createMessageBlock(hWnd, message, wParam, lParam);
            if (!isInputMessage(msg.message) /* && filterNativeEvent(&msg, result)*/)
                return false;
            // auto platformWindow = window->handle();
            // if (platformWindow && filterNativeEvent(platformWindow->window(), &msg, result))
            //     return false;
        }
        return false;
    default:
        break;
    }

    // Try hooked procedure and save result
    LRESULT result;
    // if (ctx->windowProc(hWnd, message, wParam, lParam, &result)) {
    //   // Forward the event to user-defined native event filters, there may be
    //   some
    //   // messages that need to be processed by the user.
    //   std::ignore = forwardFilteredEvent(ctx->window(), hWnd, message, wParam,
    //                                      lParam, &result);
    //   return result;
    // }

    if (customWindowHandler(hWnd, message, wParam, lParam, &result)) {
        return true;
    }

    // Continue dispatching.
    return ::CallWindowProcW(g_qtWindowProc, hWnd, message, wParam, lParam);
}

WindowHelper::WindowHelper() {}

void WindowHelper::setup(QWidget *w)
{
    auto wid = w->winId();
    auto hWnd = reinterpret_cast<HWND>(wid);
    auto style = ::GetWindowLongPtrW(hWnd, GWL_STYLE);
    ::SetWindowLongPtrW(hWnd, GWL_STYLE, style | WS_THICKFRAME | WS_CAPTION | WS_MAXIMIZEBOX);

    g_qtWindowProc = reinterpret_cast<WNDPROC>(::GetWindowLongPtrW(hWnd, GWLP_WNDPROC));

    ::SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(QWKHookedWndProc));
}
