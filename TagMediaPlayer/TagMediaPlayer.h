#ifndef TAGMEDIAPLAYER_H
#define TAGMEDIAPLAYER_H

#include <EZFrameless.h>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TagMediaPlayer;
}
QT_END_NAMESPACE

class TagMediaPlayer : public EZFrameless
{
    Q_OBJECT

public:
    TagMediaPlayer(QWidget *parent = nullptr);
    ~TagMediaPlayer();

private:
    Ui::TagMediaPlayer *ui;
};
#endif // TAGMEDIAPLAYER_H
