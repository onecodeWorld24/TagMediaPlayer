#include "TagMediaPlayer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGuiApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    TagMediaPlayer w;
    w.show();
    return a.exec();
}
