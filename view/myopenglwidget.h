#ifndef CPP4_3DVIEWER_V2_VIEW_MYOPENGLWIDGET_H
#define CPP4_3DVIEWER_V2_VIEW_MYOPENGLWIDGET_H

#define GL_SILENCE_DEPRECATION

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "../model/3d_viewer_model.h"

#pragma comment(lib, "opengl32.lib")

// namespace s21 {

class MyOpenGLWidget : public QOpenGLWidget, public QOpenGLFunctions {
  Q_OBJECT

 public:
  MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent), obj_data_(nullptr) {}

  void SetObjData(const s21::ViewerModel::Obj &obj_data);

  void SetEnablePoints(bool new_enable_points);
  void SetParallelProj(bool new_parallel_proj);
  void SetStippleLine(bool new_stipple_line);
  void SetSmoothPoint(bool new_smooth_point);
  void SetMaxCoord(double new_max_coord);
  void SetBackgroundColor(double new_red, double new_green, double new_blue);
  void SetLineColor(double new_red, double new_green, double new_blue);
  void SetVertColor(double new_red, double new_green, double new_blue);
  void SetLineWidth(int new_line_width);
  void SetVertexWidth(int new_vertex_width);

 protected:
  void initializeGL() override;
  void resizeGL(int widt = 580, int height = 362) override;
  void paintGL() override;

 private:
  const s21::ViewerModel::Obj *obj_data_;
  bool parallel_proj = false;
  bool stipple_line = false;
  bool enable_points = true;
  bool smooth_point = true;
  double max_coord = 1;
  double background_color[3] = {0, 0, 0};
  double line_color[3] = {1.0, 1.0, 1.0};
  double vert_color[3] = {1.0, 1.0, 1.0};
  int line_width = 1;
  int vertex_width = 1;
};
//}
#endif  // CPP4_3DVIEWER_V2_VIEW_MYOPENGLWIDGET_H
