#ifndef CPP4_3DVIEWER_V2_MODEL_3D_VIEWER_MODEL_H
#define CPP4_3DVIEWER_V2_MODEL_3D_VIEWER_MODEL_H

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#define S21_MX_SIZE 3
#define S21_X 0
#define S21_Y 1
#define S21_Z 2

namespace s21 {

class ViewerModel {
 public:
  /// @brief Структура, описывающая вершину 3D-модели
  struct Vertex {
    double x, y, z;  ///< координаты вершины
  };

  /// @brief Структура, описывающая полигон 3D-модели
  struct Face {
    std::vector<int> vertices;  ///< вектор индексов вершин полигона
  };

  /// @brief матрица 3х3.
  struct Matrix {
    double elements[S21_MX_SIZE][S21_MX_SIZE];
  };

  /// @brief структура хранящая данные о трансформациях объекта
  struct Transformations {
    double pos[3];  ///< Смещение объекта относительно нуля (x, y, z)
    double angle[3];  ///< углы поворота объекта (x, y, z)
    double scale;     ///< масштаб объекта
  };

  /// @brief структура хранящая данные о вершинах и полигонах из файла 3D-модели
  struct Obj {
    int vert_number;               ///< количество вершин
    int faces_number;              ///< количество полигонов
    std::vector<Vertex> vert_arr;  ///< вектор структур типа Vertex.
    std::vector<Face> face_arr;  ///< вектор структур типа Face.
    Transformations curr_state_;  ///< текущее состояние объекта (позиция, углы
                                  ///< поворота, масштаб)
    double radius;  ///< ридиус сферы, описанной вокруг объекта
  };

  ViewerModel();
  ViewerModel(const ViewerModel &other);
  ViewerModel &operator=(const ViewerModel &other);
  ~ViewerModel();

  void LoadObj(const char *filename);
  void ClearObj();

  const Obj &GetObj();

  void Translate(double x, double y, double z);
  void Scale(double scale);
  void Rotate(double x_angle, double y_angle, double z_angle);

  const Transformations &GetObjCurrentTransformations();
  void SetObjCurrentTransformations(double x, double y, double z, double x_ang,
                                    double y_ang, double z_ang, double scale);
  void ResetObjCurrentTransformations();

 private:
  int CountLines(std::ifstream &obj_file, const char *starts_with);
  void LoadVectorOfVertices(std::ifstream &obj_file);
  void LoadVectorOfFaces(std::ifstream &obj_file);
  void CenterObj();
  double FindObjRadius();
  void TransformVertexMassive(const Matrix &matrix);
  void TransformVertex(Vertex &vertex, const Matrix &matrix);
  void InitRotationMatrix(double x_angle, double y_angle, double z_angle,
                          Matrix &matrix);
  void InitScaleMatrix(double scale, Matrix &matrix);

  Obj *obj_;
};

}  // namespace s21

#endif
