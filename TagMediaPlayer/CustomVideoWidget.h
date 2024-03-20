#ifndef CUSTOMVIDEOWIDGET_H
#define CUSTOMVIDEOWIDGET_H

#include <QVideoWidget>

class CustomVideoWidget : public QVideoWidget
{
public:
    CustomVideoWidget(QWidget *parent = nullptr);

    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
};

#endif // CUSTOMVIDEOWIDGET_H
