#include "TagMediaPlayer.h"
#include "./ui_TagMediaPlayer.h"

TagMediaPlayer::TagMediaPlayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TagMediaPlayer)
{
    ui->setupUi(this);
}

TagMediaPlayer::~TagMediaPlayer()
{
    delete ui;
}

