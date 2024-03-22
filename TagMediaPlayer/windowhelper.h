#ifndef WINDOWHELPER_H
#define WINDOWHELPER_H

#include <QWidget>
#include <windows.h>

class WindowHelper {
public:
  static WindowHelper *instance() {
    static WindowHelper instance;
    return &instance;
  }

  WindowHelper();

  void setup(QWidget *w);
};

#endif // WINDOWHELPER_H
