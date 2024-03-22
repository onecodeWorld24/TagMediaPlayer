#include "TagMediaPlayer.h"
#include "./ui_TagMediaPlayer.h"
#include "windowhelper.h"

TagMediaPlayer::TagMediaPlayer(QWidget *parent)
    : EZFrameless(parent)
    , ui(new Ui::TagMediaPlayer)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    setTitlebarWidget(ui->titlebar);
    WindowHelper::instance()->setup(this);
}

TagMediaPlayer::~TagMediaPlayer()
{
    delete ui;
}
