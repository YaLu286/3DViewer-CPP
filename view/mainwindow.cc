#include "mainwindow.h"

#include "./ui_mainwindow.h"
#include "QtWidgets/qfiledialog.h"
#include "gif.h"
#include "myopenglwidget.h"

namespace s21 {

MainWindow::MainWindow(QWidget *parent, ViewerController *ctrl)
    : QMainWindow(parent), ui(new Ui::MainWindow), ctrl_(ctrl) {
  ui->setupUi(this);

  QList<QRadioButton *> radio_buttons = findChildren<QRadioButton *>();
  for (auto radio_button : radio_buttons) {
    connect(radio_button, &QRadioButton::clicked, this,
            &MainWindow::RadioClicked);
  }

  QList<QSlider *> sliders = findChildren<QSlider *>();
  for (auto slider : sliders) {
    if (slider->objectName().contains("color")) {
      connect(slider, &QSlider::valueChanged, this, &MainWindow::SetColor);
      //    } else if (slider->objectName().contains("rotation")) {
      //        connect(slider, &QSlider::valueChanged, this,
      //        &MainWindow::SliderRotation);
    } else if (slider->objectName().contains("trans")) {
      connect(slider, &QSlider::valueChanged, this,
              &MainWindow::SliderTranslation);
    }
  }
  connect(ui->load_obj_button, &QPushButton::released, this,
          &MainWindow::onLoadObjButtonClicked);
  connect(ui->save_settings, &QPushButton::released, this,
          &MainWindow::onSaveSettingsClicked);
  connect(ui->load_settings, &QPushButton::released, this,
          &MainWindow::onLoadSettingsClicked);
  connect(ui->save_in_jpeg, &QPushButton::released, this,
          &MainWindow::onSaveAsJpegClicked);
  connect(ui->save_in_bmp, &QPushButton::released, this,
          &MainWindow::onSaveAsBmpClicked);
  connect(ui->save_in_gif, &QPushButton::released, this,
          &MainWindow::onSaveAsGifClicked);
  connect(ui->reset_position, &QPushButton::released, this,
          &MainWindow::onResetPositionReleased);

  connect(ui->scaling_slider, &QSlider::sliderMoved, this,
          &MainWindow::onScalingSliderValueChanged);
  connect(ui->line_width_slider, &QSlider::sliderMoved, this,
          &MainWindow::onLineWidthSliderValueChanged);
  connect(ui->vertex_width_slider, &QSlider::sliderMoved, this,
          &MainWindow::onVertexWidthSliderValueChanged);

  connect(ui->X_rotation_slider, &QSlider::sliderMoved, this,
          &MainWindow::onXRotationSliderSliderMoved);
  connect(ui->Y_rotation_slider, &QSlider::sliderMoved, this,
          &MainWindow::onYRotationSliderSliderMoved);
  connect(ui->Z_rotation_slider, &QSlider::sliderMoved, this,
          &MainWindow::onZRotationSliderSliderMoved);
}

MainWindow::~MainWindow() { delete ui; }
void MainWindow::LoadModelFromFile(QString filename) {
  if (!filename.isEmpty()) {
    try {
      ctrl_->LoadObject(filename.toStdString());
      SetDefaults();
      ui->vertices_label->setText(
          QString::number(ctrl_->GetObjData().vert_number));
      ui->faces_label->setText(
          QString::number(ctrl_->GetObjData().faces_number));
      ui->oglwidget->SetObjData(ctrl_->GetObjData());
      ui->oglwidget->update();
    } catch (std::runtime_error &e) {
      QMessageBox msgWarning;
      msgWarning.setText(QString(e.what()));
      msgWarning.setIcon(QMessageBox::Warning);
      msgWarning.setWindowTitle("Caution");
      msgWarning.exec();
      DisableSliders();
    }
  }
}

void MainWindow::onLoadObjButtonClicked() {
  QString filename = QFileDialog::getOpenFileName(
      this, "Open a File", QDir::currentPath(), tr("Obj Files (*.Obj *.obj)"));
  QFileInfo fileinfo{filename};
  this->full_path_filename = filename;
  ui->name_label->setText(fileinfo.fileName());
  LoadModelFromFile(full_path_filename);
}
void MainWindow::DisableSliders() {
  ui->X_rotation_slider->setEnabled(false);
  ui->Y_rotation_slider->setEnabled(false);
  ui->Z_rotation_slider->setEnabled(false);
  ui->scaling_slider->setEnabled(false);
  ui->X_trans_slider->setEnabled(false);
  ui->Y_trans_slider->setEnabled(false);
  ui->Z_trans_slider->setEnabled(false);
}

void MainWindow::SetDefaults() {
  ctrl_->ObjToStartPos();
  double start_pos_Z =
      -1.0f - ctrl_->GetObjData().radius;  //////////// to fix!!
  ui->X_rotation_slider->setEnabled(true);
  ui->X_rotation_slider->setSliderPosition(0);
  ui->x_angle->setText("0°");
  ui->Y_rotation_slider->setEnabled(true);
  ui->Y_rotation_slider->setSliderPosition(0);
  ui->y_angle->setText("0°");
  ui->Z_rotation_slider->setEnabled(true);
  ui->Z_rotation_slider->setSliderPosition(0);
  ui->z_angle->setText("0°");

  ui->scaling_slider->setEnabled(true);
  ui->scaling_slider->setSliderPosition(1000);
  ui->scale->setText("1.0");

  int max_trans_pos = (int)(500 * ctrl_->GetObjData().radius);
  //                             ctrl_->GETOBJRADIUS!);  ----- TO FIX!

  ui->X_trans_slider->setEnabled(true);
  ui->X_trans_slider->setMinimum(-max_trans_pos);
  ui->X_trans_slider->setMaximum(max_trans_pos);
  ui->X_trans_slider->setSliderPosition(0);
  ui->Y_trans_slider->setEnabled(true);
  ui->Y_trans_slider->setMinimum(-max_trans_pos);
  ui->Y_trans_slider->setMaximum(max_trans_pos);
  ui->Y_trans_slider->setSliderPosition(0);
  ui->Z_trans_slider->setEnabled(true);
  ui->Z_trans_slider->setMinimum(-max_trans_pos + (int)(start_pos_Z * 100));
  ui->Z_trans_slider->setMaximum(max_trans_pos + (int)(start_pos_Z * 100));
  ui->Z_trans_slider->setSliderPosition((int)(start_pos_Z * 100));
}

//////////////// /////////////// TO FIX !!!!!!!!!!!
void MainWindow::onResetPositionReleased() {
  LoadModelFromFile(full_path_filename);
}

void MainWindow::RadioClicked() {
  QRadioButton *radio_button = qobject_cast<QRadioButton *>(sender());
  if (radio_button->objectName() == "parallel_proj") {
    ui->oglwidget->SetParallelProj(true);
  } else if (radio_button->objectName() == "central_proj") {
    ui->oglwidget->SetParallelProj(false);
  } else if (radio_button->objectName() == "not_stipple_line") {
    ui->oglwidget->SetStippleLine(false);
  } else if (radio_button->objectName() == "stipple_line") {
    ui->oglwidget->SetStippleLine(true);
  } else if (radio_button->objectName() == "circle_vert_radio") {
    ui->oglwidget->SetSmoothPoint(true);
    ui->oglwidget->SetEnablePoints(true);
  } else if (radio_button->objectName() == "square_vert_radio") {
    ui->oglwidget->SetSmoothPoint(false);
    ui->oglwidget->SetEnablePoints(true);
  } else if (radio_button->objectName() == "disable_vert_radio") {
    ui->oglwidget->SetEnablePoints(false);
  }
  ui->oglwidget->update();
}

void MainWindow::SliderRotation() {
  double new_x_angle = (double)ui->X_rotation_slider->value() / 100000;
  double new_y_angle = (double)ui->Y_rotation_slider->value() / 100000;
  double new_z_angle = (double)ui->Z_rotation_slider->value() / 100000;
  ui->x_angle->setText(QString::number((int)(new_x_angle * 180 / 3.14)) + "°");
  ui->y_angle->setText(QString::number((int)(new_y_angle * 180 / 3.14)) + "°");
  ui->z_angle->setText(QString::number((int)(new_z_angle * 180 / 3.14)) + "°");
  ctrl_->RotateObject(new_x_angle, new_y_angle, new_z_angle);
  ui->oglwidget->update();
}

void MainWindow::SliderTranslation() {
  double x_slider_pos = (double)ui->X_trans_slider->value() / 100;
  double y_slider_pos = (double)ui->Y_trans_slider->value() / 100;
  double z_slider_pos = (double)ui->Z_trans_slider->value() / 100;
  ctrl_->TranslateObject(x_slider_pos, y_slider_pos, z_slider_pos);
  ui->oglwidget->update();
}

void MainWindow::onScalingSliderValueChanged(int value) {
  double new_scale = (double)ui->scaling_slider->value() / 1000.0;
  ctrl_->ScaleObject(new_scale);
  ui->oglwidget->update();
  ui->scale->setText(QString::number(new_scale, 'f', 2));
}

void MainWindow::SetColor() {
  ui->oglwidget->SetBackgroundColor((double)ui->back_color_R->value() / 100,
                                    (double)ui->back_color_G->value() / 100,
                                    (double)ui->back_color_B->value() / 100);
  ui->oglwidget->SetLineColor((double)ui->line_color_R->value() / 100,
                              (double)ui->line_color_G->value() / 100,
                              (double)ui->line_color_B->value() / 100);
  ui->oglwidget->SetVertColor((double)ui->vert_color_R->value() / 100,
                              (double)ui->vert_color_G->value() / 100,
                              (double)ui->vert_color_B->value() / 100);
  ui->oglwidget->update();
}

void MainWindow::onLineWidthSliderValueChanged(int value) {
  ui->oglwidget->SetLineWidth(value);
  ui->oglwidget->update();
}

void MainWindow::onVertexWidthSliderValueChanged(int value) {
  ui->oglwidget->SetVertexWidth(value);
  ui->oglwidget->update();
}

void MainWindow::onSaveSettingsClicked() {
  QSettings *set =
      new QSettings(QSettings::IniFormat, QSettings::UserScope, "3D_viewer");
  set->setValue("back_R", ui->back_color_R->value());
  set->setValue("back_G", ui->back_color_G->value());
  set->setValue("back_B", ui->back_color_B->value());
  set->setValue("lines_R", ui->line_color_R->value());
  set->setValue("lines_G", ui->line_color_G->value());
  set->setValue("lines_B", ui->line_color_B->value());
  set->setValue("vertex_R", ui->vert_color_R->value());
  set->setValue("vertex_G", ui->vert_color_G->value());
  set->setValue("vertex_B", ui->vert_color_B->value());
  set->setValue("line_width", ui->line_width_slider->value());
  set->setValue("vertex_width", ui->vertex_width_slider->value());
  set->setValue("radio_stripple_line", ui->stipple_line->isChecked());
  set->setValue("radio_not_stripple_line", ui->not_stipple_line->isChecked());
  set->setValue("radio_proj_parralel", ui->parallel_proj->isChecked());
  set->setValue("radio_proj_central", ui->central_proj->isChecked());
  set->setValue("radio_circle_vert", ui->circle_vert_radio->isChecked());
  set->setValue("radio_square_vert", ui->square_vert_radio->isChecked());
  set->setValue("radio_disable_vert", ui->disable_vert_radio->isChecked());
}

void MainWindow::onLoadSettingsClicked() {
  QSettings *set =
      new QSettings(QSettings::IniFormat, QSettings::UserScope, "3D_viewer");
  if (set != nullptr) {
    ui->back_color_R->setValue(set->value("back_R", "").toInt());
    ui->back_color_G->setValue(set->value("back_G", "").toInt());
    ui->back_color_B->setValue(set->value("back_B", "").toInt());
    ui->line_width_slider->setValue(set->value("line_width", "").toInt());
    ui->oglwidget->SetLineWidth(set->value("line_width", "").toInt());
    ui->vertex_width_slider->setValue(set->value("vertex_width", "").toInt());
    ui->oglwidget->SetVertexWidth(set->value("vertex_width", "").toInt());
    ui->line_color_R->setValue(set->value("lines_R", "").toInt());
    ui->line_color_G->setValue(set->value("lines_G", "").toInt());
    ui->line_color_B->setValue(set->value("lines_B", "").toInt());
    ui->vert_color_R->setValue(set->value("vertex_R", "").toInt());
    ui->vert_color_G->setValue(set->value("vertex_G", "").toInt());
    ui->vert_color_B->setValue(set->value("vertex_B", "").toInt());
    if (set->value("radio_stripple_line", "").toBool())
      ui->stipple_line->click();
    if (set->value("radio_not_stripple_line", "").toBool())
      ui->not_stipple_line->click();
    if (set->value("radio_proj_parralel", "").toBool())
      ui->parallel_proj->click();
    if (set->value("radio_proj_central", "").toBool())
      ui->central_proj->click();
    if (set->value("radio_circle_vert", "").toBool())
      ui->circle_vert_radio->click();
    if (set->value("radio_square_vert", "").toBool())
      ui->square_vert_radio->click();
    if (set->value("radio_disable_vert", "").toBool())
      ui->disable_vert_radio->click();
  }
  ui->oglwidget->update();
}

void MainWindow::onSaveAsJpegClicked() {
  QImage image = ui->oglwidget->grabFramebuffer();
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save Image File"), QString(), tr("Images (*.jpeg)"));
  if (!fileName.isEmpty()) {
    image.save(fileName);
  }
}

void MainWindow::onSaveAsBmpClicked() {
  QImage image = ui->oglwidget->grabFramebuffer();
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save Image File"), QString(), tr("Images (*.bmp)"));
  if (!fileName.isEmpty()) {
    image.save(fileName);
  }
}

void MainWindow::onSaveAsGifClicked() {
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save Image File"), QString(), tr("Images (*.gif)"));
  std::string cpp_filename = fileName.toStdString();
  const char *c_filename = cpp_filename.c_str();
  if (!fileName.isEmpty()) {
    std::array<QImage, 50> image_arr;
    for (int i = 0; i < 50; i++) {
      QTime dieTime = QTime::currentTime().addMSecs(100);
      image_arr[i] = ui->oglwidget->grabFramebuffer();
      while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    GifWriter writer = {};
    GifBegin(&writer, c_filename, image_arr[0].width(), image_arr[0].height(),
             10);
    for (int frame = 0; frame < 50; frame++) {
      GifWriteFrame(&writer, image_arr[frame].constBits(),
                    image_arr[frame].width(), image_arr[frame].height(), 10);
    }
    GifEnd(&writer);
  }
}

void MainWindow::onXRotationSliderSliderMoved(int position) {
  ctrl_->RotateObject(position * 3.14 / 180, 0, 0);
  ui->oglwidget->update();
  ui->x_angle->setText(QString::number(position) + "°");
}

void MainWindow::onYRotationSliderSliderMoved(int position) {
  ctrl_->RotateObject(0, position * 3.14 / 180, 0);
  ui->oglwidget->update();
  ui->y_angle->setText(QString::number(position) + "°");
}

void MainWindow::onZRotationSliderSliderMoved(int position) {
  ctrl_->RotateObject(0, 0, position * 3.14 / 180);
  ui->oglwidget->update();
  ui->z_angle->setText(QString::number(position) + "°");
}

}  // namespace s21
