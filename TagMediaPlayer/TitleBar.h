#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <EZTitleBarBase.h>
#include <QWidget>

namespace Ui {
class TitleBar;
}

class TitleBar : public EZTitleBarBase
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = nullptr);
    ~TitleBar();

    virtual bool isMovableArea(const QPoint &pt) override;

    virtual void onMaximized() override;
    virtual void onNormaled() override;

protected:
    void initTitlebar();

private:
    Ui::TitleBar *ui;
};

#endif // TITLEBAR_H
