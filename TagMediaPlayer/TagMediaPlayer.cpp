#include "TagMediaPlayer.h"
#include "./ui_TagMediaPlayer.h"

TagMediaPlayer::TagMediaPlayer(QWidget *parent)
    : EZFrameless(parent)
    , ui(new Ui::TagMediaPlayer)
{
    ui->setupUi(this);

    setTitlebarWidget(ui->titlebar);
    //    ui->playerWidget->installEventFilter(this);
}

TagMediaPlayer::~TagMediaPlayer()
{
    delete ui;
}
