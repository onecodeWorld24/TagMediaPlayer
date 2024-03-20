#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QMediaPlayer>
#include <QWidget>

namespace Ui {
class PlayerWidget;
}

class PlayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerWidget(QWidget *parent = nullptr);
    ~PlayerWidget();

protected:
    void initUI();
    void initSignals();

private slots:
    void on_btnOpenFile_clicked();

    void on_btnNext_clicked();

    void on_btnPrevious_clicked();

    void on_btnStopPlay_clicked();

    void on_btnPlay_clicked();

private:
    Ui::PlayerWidget *ui;

    QMediaPlayer *m_player;
};

#endif // PLAYERWIDGET_H
