#ifndef FRAMEEVENTFILTER_H
#define FRAMEEVENTFILTER_H

#include <QAbstractNativeEventFilter>
#include <QMutex>
#include <QWidget>
#include <QWriteLocker>

class FrameEventFilter : public QAbstractNativeEventFilter
{
public:
    enum EventType { ET_MouseSize = 0 };
    explicit FrameEventFilter();

    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;

    void setMainWindow(QWidget *widget) { m_mainWindow = widget; }

    void addUnblockedWidget(QWidget *widget)
    {
        // 注意：winId()函数只能在主线程调用，非主线程调用会触发CreateWindowEx，导致异常
        m_unblockedWid.append(widget->winId());
        m_bInited = true;
    }

protected:
    bool isUnblockedMessageWindow(WId wid);

private:
    QWidget   *m_mainWindow;
    QList<WId> m_unblockedWid;
    bool       m_bInited{false};
};

#endif // FRAMEEVENTFILTER_H
