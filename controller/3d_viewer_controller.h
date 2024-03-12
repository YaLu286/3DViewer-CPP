#ifndef CPP4_3DVIEWER_V2_MODEL_3D_VIEWER_CONTROLLER_H
#define CPP4_3DVIEWER_V2_MODEL_3D_VIEWER_CONTROLLER_H

#include "../model/3d_viewer_model.h"

namespace s21 {

class ViewerController {
 public:
  explicit ViewerController(ViewerModel *vm);
  ~ViewerController();
  void LoadObject(const std::string &filename);
  const ViewerModel::Obj &GetObjData();
  void ObjToStartPos();
  void TranslateObject(double x_slider_pos, double y_slider_pos,
                       double z_slider_pos);
  void RotateObject(double x_slider_ang, double y_slider_ang,
                    double z_slider_ang);
  void ScaleObject(double scale);

 private:
  ViewerModel *vm_;  ///< указатель на модель
};

}  // namespace s21

#endif
