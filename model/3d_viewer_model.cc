#include "3d_viewer_model.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

namespace s21 {
#define MX_SIZE 3

/// @brief Базовый конструктор
ViewerModel::ViewerModel() { obj_ = new Obj; }

/// @brief Конструктор копирования
ViewerModel::ViewerModel(const ViewerModel &other) {
  obj_ = new Obj;
  *this = other;
}

/// @brief Перегрузка оператора присваивания(копирования)
ViewerModel &ViewerModel::operator=(const ViewerModel &other) {
  *obj_ = *(other.obj_);
  return *this;
}

/// @brief Декструктор
ViewerModel::~ViewerModel() {
  if (obj_) {
    delete obj_;
  }
}

/**
 * @brief Загружает информацию из файла в структуру типа Obj
 * @param filename строка, содержащая имя файла
 */
void ViewerModel::LoadObj(const char *filename) {
  std::ifstream obj_file;
  obj_file.open(filename);
  if (obj_file.is_open()) {
    int v_num = CountLines(obj_file, "v ");
    int f_num = CountLines(obj_file, "f ");
    if (v_num && f_num) {
      ClearObj();
      obj_->vert_number = v_num;
      obj_->faces_number = f_num;
      LoadVectorOfVertices(obj_file);
      LoadVectorOfFaces(obj_file);
      CenterObj();
      obj_->radius = FindObjRadius();
    } else {
      obj_file.close();
      throw(std::runtime_error("Corrupted object file"));
    }
    obj_file.close();
  } else {
    throw(std::runtime_error("Error occured while opening file"));
  }
}

/**
 * @brief сбрасывает данные структуры типа Obj.
 */
void ViewerModel::ClearObj() {
  if (obj_) {
    obj_->face_arr.clear();
    obj_->vert_arr.clear();
    obj_->vert_number = 0;
    obj_->faces_number = 0;
    ResetObjCurrentTransformations();
  }
}

/**
 * @brief функция для подсчета строк в файле obj_file, начинающихся
 * на start_with
 * @param obj_file ссылка на поток файл
 * @param start_with строка, с которой должны начинаться искомые строки в файле
 * @return возвращает количество найденных строк
 */
int ViewerModel::CountLines(std::ifstream &obj_file, const char *s) {
  int counter = 0;
  std::string line;
  while (std::getline(obj_file, line)) {
    if (line.find(s) == 0) {
      counter++;
    }
  }
  obj_file.clear();
  obj_file.seekg(0);
  return counter;
}
/**
 * @brief функция для заполнения вектора вершин типа Vertex
 * @param obj_file ссылка на поток файла .obj
 */
void ViewerModel::LoadVectorOfVertices(std::ifstream &obj_file) {
  std::string line;
  while (std::getline(obj_file, line)) {
    if (line.find("v ") == 0) {
      double x, y, z;
      if (sscanf(line.c_str(), "%*s %lf %lf %lf", &x, &y, &z) == 3) {
        Vertex v{x, y, z};
        obj_->vert_arr.push_back(v);
      }
    }
  }
  obj_file.clear();
  obj_file.seekg(obj_file.beg);
}

/**
 * @brief функция для заполения вектора структур полигонов типа Face
 * @param obj_file ссылка на поток файла .obj
 */
void ViewerModel::LoadVectorOfFaces(std::ifstream &obj_file) {
  std::string line;
  while (std::getline(obj_file, line)) {
    if (line.find("f ") == 0) {
      Face f;
      int v_ind = 0;
      std::string temp_str;
      std::stringstream ss;
      ss << line;
      while (!ss.eof()) {
        ss >> temp_str;
        if (std::stringstream(temp_str) >> v_ind &&
            v_ind <= obj_->vert_number) {
          f.vertices.push_back(v_ind - 1);
        }
        temp_str = "";
      }
      obj_->face_arr.push_back(f);
    }
  }
  obj_file.seekg(obj_file.beg);
}

/**
 * @brief перемещает объект на заданные значения.
 * @param x - сдвиг по оси x.
 * @param y - сдвиг по оси y.
 * @param z - сдвиг по оси z.
 */
void ViewerModel::Translate(double x, double y, double z) {
  for (auto it = obj_->vert_arr.begin(); it != obj_->vert_arr.end(); it++) {
    Vertex new_coord = *it;
    new_coord.x += x;
    new_coord.y += y;
    new_coord.z += z;
    *it = new_coord;
  }
}

/**
 * @brief функция для вращения объекта
 * @param x_angle - угол поворота по оси x.
 * @param y_angle - угол поворота по оси y.
 * @param z_angle - угол поворота по оси z.
 */
void ViewerModel::Rotate(double x_angle, double y_angle, double z_angle) {
  Matrix rotation_matrix;
  InitRotationMatrix(x_angle, y_angle, z_angle, rotation_matrix);
  TransformVertexMassive(rotation_matrix);
}

/**
 * @brief функция для масштабирования объекта
 * @param scale - масштаб для изменения.
 */
void ViewerModel::Scale(double scale) {
  Matrix scale_matrix;
  InitScaleMatrix(scale, scale_matrix);
  TransformVertexMassive(scale_matrix);
}

/// @brief Получает данные об объекте
const ViewerModel::Obj &ViewerModel::GetObj() { return *obj_; }

/// @brief получает данные о трасформациях объекта
const ViewerModel::Transformations &
ViewerModel::GetObjCurrentTransformations() {
  return obj_->curr_state_;
}
/**
 * @brief устанавливает данные о трасформациях объекта
 * @param x, y, z значения пермещения по осям
 * @param x_ang, y_ang, z_ang значения углов поворота вдоль осей
 * @param значение масштаба
 */
void ViewerModel::SetObjCurrentTransformations(double x, double y, double z,
                                               double x_ang, double y_ang,
                                               double z_ang, double scale) {
  Transformations new_state = {{x, y, z}, {x_ang, y_ang, z_ang}, scale};
  obj_->curr_state_ = new_state;
}

/// @brief сбрасывает данные о трасформациях объекта до значений по умолчанию
void ViewerModel::ResetObjCurrentTransformations() {
  Transformations zero_state = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 1.0};
  obj_->curr_state_ = zero_state;
}

/// @brief помещает объект в 0
void ViewerModel::CenterObj() {
  double x_min, x_max, y_min, y_max, z_min, z_max;
  x_min = x_max = obj_->vert_arr[0].x;
  y_min = y_max = obj_->vert_arr[0].y;
  z_min = z_max = obj_->vert_arr[0].z;
  for (auto it = obj_->vert_arr.begin(); it != obj_->vert_arr.end(); it++) {
    x_min = std::min(x_min, (*it).x);
    x_max = std::max(x_max, (*it).x);
    y_min = std::min(y_min, (*it).y);
    y_max = std::max(y_max, (*it).y);
    z_min = std::min(z_min, (*it).z);
    z_max = std::max(z_max, (*it).z);
  }
  Translate(-(x_max + x_min) / 2.0, -(y_max + y_min) / 2.0,
            -(z_max + z_min) / 2.0);
}

/**
 * @brief Функция для нахождения радиуса (максимальной координаты) объекта
 * @return радиус сферы, описанной вокруг объекта
 */
double ViewerModel::FindObjRadius() {
  double max = 0.0f;
  for (auto it = obj_->vert_arr.begin(); it != obj_->vert_arr.end(); it++) {
    double abs_x = std::abs((*it).x);
    double abs_y = std::abs((*it).y);
    double abs_z = std::abs((*it).z);
    double max_coord = std::max({abs_x, abs_y, abs_z});
    if (max_coord > max) max = max_coord;
  }
  return max;
}

/**
 * @brief Функция изменяющая координаты структуры согласно поданой матрице.
 * @param ссылку на матрицу преобразований
 */
void ViewerModel::TransformVertexMassive(const Matrix &matrix) {
  for (int i = 0; i < obj_->vert_number; i++) {
    TransformVertex(obj_->vert_arr[i], matrix);
  }
}

/**
 * @brief Преобразует вектор матрицей.
 * @param *matrix - матрица преобразования.
 */
void ViewerModel::TransformVertex(Vertex &vertex, const Matrix &matrix) {
  const double curr_coord[MX_SIZE] = {vertex.x, vertex.y, vertex.z};
  double new_coord[MX_SIZE] = {0.0};
  for (int i = 0; i < MX_SIZE; i++) {
    for (int j = 0; j < MX_SIZE; j++) {
      new_coord[i] += matrix.elements[i][j] * curr_coord[j];
    }
  }
  vertex = {new_coord[0], new_coord[1], new_coord[2]};
}

/**
 * @brief Функция определяющая матрицу трансформации через поданные углы
 * вращения.
 * @param x_angle - угол поворота по оси x.
 * @param y_angle - угол поворота по оси y.
 * @param z_angle - угол поворота по оси z.
 * @param matrix - ссылка на матрицу для инициализации.
 */
void ViewerModel::InitRotationMatrix(double x_angle, double y_angle,
                                     double z_angle, Matrix &matrix) {
  matrix.elements[0][0] = cos(y_angle) * cos(z_angle);
  matrix.elements[0][1] = -sin(z_angle) * cos(y_angle);
  matrix.elements[0][2] = sin(y_angle);
  matrix.elements[1][0] =
      sin(x_angle) * sin(y_angle) * cos(z_angle) + sin(z_angle) * cos(x_angle);
  matrix.elements[1][1] =
      -sin(x_angle) * sin(y_angle) * sin(z_angle) + cos(x_angle) * cos(z_angle);
  matrix.elements[1][2] = -sin(x_angle) * cos(y_angle);
  matrix.elements[2][0] =
      sin(x_angle) * sin(z_angle) - sin(y_angle) * cos(x_angle) * cos(z_angle);
  matrix.elements[2][1] =
      sin(x_angle) * cos(z_angle) + sin(y_angle) * sin(z_angle) * cos(x_angle);
  matrix.elements[2][2] = cos(x_angle) * cos(y_angle);
}

/**
 * @brief Функция добавляющая матрице трансформации масштаб.
 * @param scale - масштаб для изменения.
 * @param *matrix - ссылка на матрицу, с которой будет проводиться скейлинг.
 * @return void
 */
void ViewerModel::InitScaleMatrix(double scale, Matrix &matrix) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < MX_SIZE; j++) {
      if (i == j) {
        matrix.elements[i][j] = scale;
      } else {
        matrix.elements[i][j] = 0.0;
      }
    }
  }
}

}  // namespace s21
