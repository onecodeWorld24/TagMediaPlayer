#include "PlayerWidget.h"
#include "ui_PlayerWidget.h"

#include <FontManager.h>
#include <QEvent>
#include <QEventForwarder.h>
#include <QFileDialog>

PlayerWidget::PlayerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayerWidget)
    , m_player(new QMediaPlayer(this))
{
    ui->setupUi(this);
    //    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Widget);

    initUI();
    initSignals();
}

PlayerWidget::~PlayerWidget()
{
    delete ui;
}

void PlayerWidget::initUI()
{
    //    ui->videoWidget->installEventFilter(this);
    //    ui->videoWidget->setMouseTracking(false);
    //    ui->videoWidget->setAutoFillBackground(true);
    m_player->setVideoOutput(ui->videoWidget);
}

void PlayerWidget::initSignals() {}

void PlayerWidget::on_btnOpenFile_clicked()
{
    auto videoFile = QFileDialog::getOpenFileName(this, "open", "E://", "*.mp4");
    m_player->setMedia(QUrl::fromLocalFile(videoFile));
    m_player->play();
}

void PlayerWidget::on_btnNext_clicked() {}

void PlayerWidget::on_btnPrevious_clicked() {}

void PlayerWidget::on_btnStopPlay_clicked() {}

void PlayerWidget::on_btnPlay_clicked() {}
