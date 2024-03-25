#include "FrameEventFilter.h"
#include "EZFrameless.h"
#include <QEvent>
#include <QMouseEvent>
#include <QReadLocker>
#include <QSplitter>
#include <QWindow>

#include <windows.h>
//
#include <WinUser.h>
#include <dwmapi.h>
#include <windowsx.h>

FrameEventFilter::FrameEventFilter()
    : QAbstractNativeEventFilter()
{}

bool FrameEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    if (eventType == "windows_generic_MSG") {
        MSG *msg = static_cast<MSG *>(message);
        if ((msg->message == WM_NCHITTEST || msg->message == WM_NCCALCSIZE)
            && (WId) msg->hwnd != m_mainWindow->winId()) {
            if (isUnblockedMessageWindow((WId) msg->hwnd))
                return false;
            *result = HTTRANSPARENT;
            return true;
        }
    }
    return false; // 返回false让事件继续传递
}

bool FrameEventFilter::isUnblockedMessageWindow(WId wid)
{
    if (!m_bInited)
        return true;
    QReadLocker locker(&m_mutex);
    foreach (WId widget, m_unblockedWid) {
        if (widget == wid)
            return true;
    }

    return false;
}
