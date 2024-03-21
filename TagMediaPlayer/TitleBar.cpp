#include "TitleBar.h"
#include "GenericMacros.h"
#include "ui_TitleBar.h"

#include <FontManager.h>

using namespace qutils;

TitleBar::TitleBar(QWidget *parent)
    : EZTitleBarBase(parent)
    , ui(new Ui::TitleBar)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    initTitlebar();
}

TitleBar::~TitleBar()
{
    delete ui;
}

bool TitleBar::isMovableArea(const QPoint &pt)
{
    if (rect().contains(pt)) {
        if (ui->btnSetting->rect().contains(ui->btnSetting->mapFromParent(pt))
            || ui->btnMin->rect().contains(ui->btnMin->mapFromParent(pt))
            || ui->btnMax->rect().contains(ui->btnMax->mapFromParent(pt))
            || ui->btnClose->rect().contains(ui->btnClose->mapFromParent(pt))) {
            return false;
        }
        return true;
    }
    return false;
}

void TitleBar::onMaximized()
{
    ui->btnMax->setText(QChar(0xe65b));
}

void TitleBar::onNormaled()
{
    ui->btnMax->setText(QChar(0xe64c));
}

void TitleBar::initTitlebar()
{
    QFont iconFont = FontManager::instance()->fontAt(FontType::kIconFont);

    ui->btnClose->setFont(iconFont);
    ui->btnClose->setText(QChar(0xe6fc));
    ui->btnMax->setFont(iconFont);
    ui->btnMax->setText(QChar(0xe64c));
    ui->btnMin->setFont(iconFont);
    ui->btnMin->setText(QChar(0xe60b));
    ui->btnSetting->setFont(iconFont);
    ui->btnSetting->setText(QChar(0xe6fa));

    connect(ui->btnClose, &QPushButton::clicked, this, &EZTitleBarBase::onClosedClicked);
    connect(ui->btnMax, &QPushButton::clicked, this, &EZTitleBarBase::onMaximizedClicked);
    connect(ui->btnMin, &QPushButton::clicked, this, &EZTitleBarBase::onMinimizedClicked);
}
