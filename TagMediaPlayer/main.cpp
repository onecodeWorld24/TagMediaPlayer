#include "FrameEventFilter.h"
#include "GenericMacros.h"
#include "TagMediaPlayer.h"

#include <FontManager.h>
#include <QApplication>
#include <QDebug>
#include <QFile>

bool loadStyles(QApplication &a, const QString &path);
bool loadCustomFonts();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGuiApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    // FrameEventFilter *eventFilter = new FrameEventFilter();

    if (!loadStyles(a, "://res/style.css"))
        return 0;

    if (!loadCustomFonts())
        return 0;

    TagMediaPlayer w;
    // eventFilter->setMainWindow(&w);
    // a.installNativeEventFilter(eventFilter);
    w.show();
    return a.exec();
}

bool loadStyles(QApplication &a, const QString &path)
{
    QFile   file(path);
    QString styleSheet = "";
    if (file.open(QFile::ReadOnly)) {
        styleSheet = QLatin1String(file.readAll());

        a.setStyleSheet(styleSheet);
        file.close();
    }
}

bool loadCustomFonts()
{
    bool ret = qutils::FontManager::instance()->addThirdpartyFont("://res/font/iconfont.ttf", FontType::kIconFont);

    return ret;
}
