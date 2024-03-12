#include "3d_viewer_controller.h"

#include <iostream>

namespace s21 {

/// @brief Базовый конструктор
ViewerController::ViewerController(ViewerModel *vm) : vm_(vm) {}

/// @brief  Деструктор
ViewerController::~ViewerController() {}

/// @brief Вызывает в модели метод загрузки объекта
/// @param filename строка, содержащая путь до файла
void ViewerController::LoadObject(const std::string &filename) {
  vm_->LoadObj(filename.c_str());
}

const ViewerModel::Obj &ViewerController::GetObjData() { return vm_->GetObj(); }

/// @brief Перемещает объект на начальную позицию (перед наблюдателем)
void ViewerController::ObjToStartPos() {
  double z_start_pos = -1 - vm_->GetObj().radius;
  vm_->Translate(0.0f, 0.0f, z_start_pos);
  vm_->SetObjCurrentTransformations(0.0, 0.0, z_start_pos, 0.0, 0.0, 0.0, 1.0);
}

/// @brief Перемещает объект на величину равную разнице позиций слайдеров и
/// текущего положения объекта
/// @param x_slider_pos позиция слайдера, отвечающего за перемещение по оси x
/// @param y_slider_pos позиция слайдера, отвечающего за перемещение по оси y
/// @param z_slider_pos позиция слайдера, отвечающего за перемещение по оси z
void ViewerController::TranslateObject(double x_slider_pos, double y_slider_pos,
                                       double z_slider_pos) {
  ViewerModel::Transformations curr_trans = vm_->GetObjCurrentTransformations();

  double x_trans_value = x_slider_pos - curr_trans.pos[S21_X];
  double y_trans_value = y_slider_pos - curr_trans.pos[S21_Y];
  double z_trans_value = z_slider_pos - curr_trans.pos[S21_Z];

  vm_->Translate(x_trans_value, y_trans_value, z_trans_value);

  vm_->SetObjCurrentTransformations(
      x_slider_pos, y_slider_pos, z_slider_pos, curr_trans.angle[S21_X],
      curr_trans.angle[S21_Y], curr_trans.angle[S21_Z], curr_trans.scale);
}

/// @brief Вращает объект на величину равную разнице позиций слайдеров и текущих
/// углов поворота объекта
/// @param x_slider_ang позиция слайдера, отвечающего за вращения вдоль оси x
/// @param y_slider_ang позиция слайдера, отвечающего за вращения вдоль оси y
/// @param z_slider_ang позиция слайдера, отвечающего за вращения вдоль оси z
void ViewerController::RotateObject(double x_slider_ang, double y_slider_ang,
                                    double z_slider_ang) {
  ViewerModel::Transformations curr_trans = vm_->GetObjCurrentTransformations();

  double x_rotation_value = x_slider_ang - curr_trans.angle[0];
  double y_rotation_value = y_slider_ang - curr_trans.angle[1];
  double z_rotation_value = z_slider_ang - curr_trans.angle[2];

  vm_->Translate(-1.0 * curr_trans.pos[0], -1.0 * curr_trans.pos[1],
                 -1.0 * curr_trans.pos[2]);
  vm_->Rotate(x_rotation_value, y_rotation_value, z_rotation_value);
  vm_->Translate(curr_trans.pos[S21_X], curr_trans.pos[S21_Y],
                 curr_trans.pos[S21_Z]);

  vm_->SetObjCurrentTransformations(
      curr_trans.pos[S21_X], curr_trans.pos[S21_Y], curr_trans.pos[S21_Z],
      x_slider_ang, y_slider_ang, z_slider_ang, curr_trans.scale);
}

void ViewerController::ScaleObject(double new_scale) {
  ViewerModel::Transformations curr_trans = vm_->GetObjCurrentTransformations();

  double scaling_value = new_scale / curr_trans.scale;

  vm_->Translate(-curr_trans.pos[S21_X], -curr_trans.pos[S21_Y],
                 -curr_trans.pos[S21_Z]);
  vm_->Scale(scaling_value);
  vm_->Translate(curr_trans.pos[S21_X], curr_trans.pos[S21_Y],
                 curr_trans.pos[S21_Z]);

  vm_->SetObjCurrentTransformations(
      curr_trans.pos[S21_X], curr_trans.pos[S21_Y], curr_trans.pos[S21_Z],
      curr_trans.angle[S21_X], curr_trans.angle[S21_Y], curr_trans.angle[S21_Z],
      new_scale);
}

}  // namespace s21
