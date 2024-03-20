#include "TitleBar.h"
#include "ui_TitleBar.h"

TitleBar::TitleBar(QWidget *parent)
    : EZTitleBarBase(parent)
    , ui(new Ui::TitleBar)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TransparentForMouseEvents);
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
