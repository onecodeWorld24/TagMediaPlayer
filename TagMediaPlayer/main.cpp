#include "TagMediaPlayer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TagMediaPlayer w;
    w.show();
    return a.exec();
}
