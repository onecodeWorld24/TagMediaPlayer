#include "CustomVideoWidget.h"
#include <windows.h>
//
#include <WinUser.h>
#include <dwmapi.h>
#include <windowsx.h>

CustomVideoWidget::CustomVideoWidget(QWidget *parent)
    : QVideoWidget(parent)
{}

bool CustomVideoWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    //    MSG *mes = reinterpret_cast<MSG *>(message);
    //    if (mes->message == WM_PAINT || mes->message == WM_ERASEBKGND) {
    //        if (d->windowBackend)
    //            d->windowBackend->showEvent();
    //    }

    return false;
}
