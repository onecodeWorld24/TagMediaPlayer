#include "PlayerWidget.h"
#include "GenericMacros.h"
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
    setAttribute(Qt::WA_StyledBackground);

    initUI();
    initSignals();
}

PlayerWidget::~PlayerWidget()
{
    delete ui;
}

void PlayerWidget::initUI()
{
    m_player->setVideoOutput(ui->videoWidget);

    auto iconFont = qutils::FontManager::instance()->fontAt(FontType::kIconFont);
    ui->btnFastBack->setFont(iconFont);
    ui->btnFastBack->setText(QChar(0xe6f4));
    ui->btnFastForward->setFont(iconFont);
    ui->btnFastForward->setText(QChar(0xe6ef));
    iconFont.setPointSize(iconFont.pointSize() + 1);
    ui->btnPlay->setFont(iconFont);
    ui->btnPlay->setText(QChar(0xe6b7));
    ui->btnAddTag->setFont(iconFont);
    ui->btnAddTag->setText(QChar(0xe6d9));
    ui->btnNext->setFont(iconFont);
    ui->btnNext->setText(QChar(0xe6b6));
    ui->btnOpenFile->setFont(iconFont);
    ui->btnOpenFile->setText(QChar(0xe6fd));
    ui->btnPrevious->setFont(iconFont);
    ui->btnPrevious->setText(QChar(0xe6f7));
    ui->btnStopPlay->setFont(iconFont);
    ui->btnStopPlay->setText(QChar(0xe6b9));
    ui->btnVolume->setFont(iconFont);
    ui->btnVolume->setText(QChar(0xe6bd));
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
