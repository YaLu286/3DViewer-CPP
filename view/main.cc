#include <locale.h>

#include <QApplication>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextEdit>

#include "../controller/3d_viewer_controller.h"
#include "../model/3d_viewer_model.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  setlocale(LC_ALL, "en_US.utf8");
  s21::ViewerModel vm;
  s21::ViewerController vctrl(&vm);
  s21::MainWindow w(nullptr, &vctrl);
  w.show();
  return a.exec();
}
