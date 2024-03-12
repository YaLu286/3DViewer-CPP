#include "myopenglwidget.h"
// namespace s21 {

void MyOpenGLWidget::SetObjData(const s21::ViewerModel::Obj &obj_data) {
  obj_data_ = &obj_data;
}

void MyOpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(background_color[0], background_color[1], background_color[2],
               0);
  glEnable(GL_DEPTH_TEST);
}

void MyOpenGLWidget::resizeGL(int width, int height) {
  width *= this->devicePixelRatio();
  height *= this->devicePixelRatio();
  double zNear = 0.1f;

  glViewport(0, 0, (GLint)width, (GLint)height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (obj_data_) {
    if (parallel_proj) {
      double height_o = obj_data_->radius * 1.5f;
      double width_o = height_o * this->width() / this->height();
      glOrtho(-width_o, width_o, -height_o, height_o, -0.5f,
              obj_data_->radius * 5);
    } else {
      double height_f = zNear;
      double width_f = height_f * this->width() / this->height();
      glFrustum(-width_f, width_f, -height_f, height_f, zNear,
                obj_data_->radius * 10);
    }
  }
}

void MyOpenGLWidget::paintGL() {
  glClearColor(background_color[0], background_color[1], background_color[2],
               0);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (obj_data_) {
    qDebug() << "PAINT";
    resizeGL();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //    for(int i = 1; i < obj_data.vert_number; i++) {
    //        for(int j = 0; j < 3; j++) {
    //            qDebug() << *((double *)obj_data.vert_arr->data() + i*j);
    //        }
    //    }

    glVertexPointer(3, GL_DOUBLE, 0, (double *)obj_data_->vert_arr.data());
    glEnableClientState(GL_VERTEX_ARRAY);

    if (stipple_line) {
      glLineStipple(1, 0x00FF);
      glEnable(GL_LINE_STIPPLE);
    } else {
      glDisable(GL_LINE_STIPPLE);
    }

    if (smooth_point) {
      glEnable(GL_POINT_SMOOTH);
    } else {
      glDisable(GL_POINT_SMOOTH);
    }

    for (int i = 0; i < obj_data_->faces_number; i++) {
      int face_len = (int)obj_data_->face_arr[i].vertices.size();
      unsigned int *vert_ind_arr =
          (unsigned int *)obj_data_->face_arr[i].vertices.data();
      glLineWidth(line_width);
      glColor3f(line_color[0], line_color[1], line_color[2]);
      //      for(int i = 0; i < face_len; i++) {
      //          qDebug() << *(vert_ind_arr + i);
      //      }
      glDrawElements(GL_LINE_LOOP, face_len, GL_UNSIGNED_INT, vert_ind_arr);

      if (enable_points) {
        glPointSize(vertex_width);
        glColor3f(vert_color[0], vert_color[1], vert_color[2]);
        glDrawElements(GL_POINTS, face_len, GL_UNSIGNED_INT, vert_ind_arr);
      }
    }
  }
}

void MyOpenGLWidget::SetParallelProj(bool new_parallel_proj) {
  parallel_proj = new_parallel_proj;
}

void MyOpenGLWidget::SetStippleLine(bool new_stipple_line) {
  stipple_line = new_stipple_line;
}

void MyOpenGLWidget::SetEnablePoints(bool new_enable_points) {
  enable_points = new_enable_points;
}

void MyOpenGLWidget::SetSmoothPoint(bool new_smooth_point) {
  smooth_point = new_smooth_point;
}

void MyOpenGLWidget::SetMaxCoord(double new_max_coord) {
  max_coord = new_max_coord;
}

void MyOpenGLWidget::SetBackgroundColor(double new_red, double new_green,
                                        double new_blue) {
  background_color[0] = new_red;
  background_color[1] = new_green;
  background_color[2] = new_blue;
}

void MyOpenGLWidget::SetLineColor(double new_red, double new_green,
                                  double new_blue) {
  line_color[0] = new_red;
  line_color[1] = new_green;
  line_color[2] = new_blue;
}

void MyOpenGLWidget::SetVertColor(double new_red, double new_green,
                                  double new_blue) {
  vert_color[0] = new_red;
  vert_color[1] = new_green;
  vert_color[2] = new_blue;
}

void MyOpenGLWidget::SetLineWidth(int new_line_width) {
  line_width = new_line_width;
}

void MyOpenGLWidget::SetVertexWidth(int new_vertex_width) {
  vertex_width = new_vertex_width;
}

//}
