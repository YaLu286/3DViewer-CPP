#ifndef CPP4_3DVIEWER_V2_VIEW_MAINWINDOW_H
#define CPP4_3DVIEWER_V2_VIEW_MAINWINDOW_H

#define S21_RED 0
#define S21_GREEN 1
#define S21_BLUE 2

#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QTime>
#include <QWidget>

#include "../controller/3d_viewer_controller.h"
#include "myopenglwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent, ViewerController *ctrl);
  ~MainWindow();

 private slots:
  void SliderRotation();
  void SliderTranslation();
  void onScalingSliderValueChanged(int value);
  void RadioClicked();
  void SetDefaults();
  void SetColor();
  void onLoadObjButtonClicked();
  void onSaveSettingsClicked();
  void onLoadSettingsClicked();
  void onLineWidthSliderValueChanged(int value);
  void onVertexWidthSliderValueChanged(int value);
  void onSaveAsJpegClicked();
  void onSaveAsBmpClicked();
  void onSaveAsGifClicked();
  void onResetPositionReleased();

  void onXRotationSliderSliderMoved(int position);
  void onYRotationSliderSliderMoved(int position);
  void onZRotationSliderSliderMoved(int position);

 private:
  void DisableSliders();
  void LoadModelFromFile(QString filename);
  QString full_path_filename;
  Ui::MainWindow *ui;
  ViewerController *ctrl_;
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_VIEW_MAINWINDOW_H
