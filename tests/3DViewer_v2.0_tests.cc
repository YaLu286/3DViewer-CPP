#include <gtest/gtest.h>

#include "../model/3d_viewer_model.h"

TEST(Viewer_v2, init_viewer) {
  s21::ViewerModel test;
  test.LoadObj("tests/models/cube.obj");
  s21::ViewerModel::Obj test_obj = test.GetObj();
  ASSERT_EQ(test_obj.vert_number, 8);
  ASSERT_EQ(test_obj.faces_number, 12);
}

TEST(Viewer_v2, create_trough_copy_viewer) {
  s21::ViewerModel test;
  test.LoadObj("tests/models/cube.obj");
  s21::ViewerModel test_2(test);
  s21::ViewerModel::Obj test_obj = test_2.GetObj();
  ASSERT_EQ(test_obj.vert_number, 8);
  ASSERT_EQ(test_obj.faces_number, 12);
}

TEST(Viewer_v2, copy_viewer) {
  s21::ViewerModel test, test_2;
  test.LoadObj("tests/models/cube.obj");
  test_2.LoadObj("tests/models/building.obj");
  s21::ViewerModel::Obj test_obj = test_2.GetObj();
  ASSERT_EQ(test_obj.vert_number, 3194);
  ASSERT_EQ(test_obj.faces_number, 1825);
  test_2 = test;
  test_obj = test_2.GetObj();
  ASSERT_EQ(test_obj.vert_number, 8);
  ASSERT_EQ(test_obj.faces_number, 12);
}

TEST(Viewer_v2, load_error_1) {
  s21::ViewerModel test;
  EXPECT_THROW(test.LoadObj("tests/models/cubsse.obj"), std::runtime_error);
}

TEST(Viewer_v2, load_error_2) {
  s21::ViewerModel test;
  EXPECT_THROW(test.LoadObj("tests/models/test_0.obj"), std::runtime_error);
}

TEST(Viewer_v2, clear_obj) {
  s21::ViewerModel test;
  test.LoadObj("tests/models/cube.obj");
  s21::ViewerModel::Obj test_obj = test.GetObj();
  ASSERT_EQ(test_obj.vert_number, 8);
  ASSERT_EQ(test_obj.faces_number, 12);
  test.ClearObj();
  test_obj = test.GetObj();
  ASSERT_EQ(test_obj.vert_number, 0);
  ASSERT_EQ(test_obj.faces_number, 0);
  ASSERT_EQ(test_obj.face_arr.size(), 0);
  ASSERT_EQ(test_obj.vert_arr.size(), 0);
}

TEST(Viewer_v2, rotate) {
  s21::ViewerModel test;
  test.LoadObj("tests/models/cube.obj");
  s21::ViewerModel::Obj test_obj = test.GetObj();
  ASSERT_NEAR(test_obj.vert_arr[0].x, 1, 0.000001);
  ASSERT_NEAR(test_obj.vert_arr[0].y, -1, 0.000001);
  ASSERT_NEAR(test_obj.vert_arr[0].z, -1, 0.000001);
  test.Rotate(1, 1, 1);
  test_obj = test.GetObj();
  ASSERT_NEAR(test_obj.vert_arr[0].x, -0.094896, 0.000001);
  ASSERT_NEAR(test_obj.vert_arr[0].y, 1.595768, 0.000001);
  ASSERT_NEAR(test_obj.vert_arr[0].z, -0.666723, 0.000001);
}

TEST(Viewer_v2, scale) {
  s21::ViewerModel test;
  test.LoadObj("tests/models/cube.obj");
  s21::ViewerModel::Obj test_obj = test.GetObj();
  ASSERT_NEAR(test_obj.vert_arr[0].x, 1, 0.00001);
  ASSERT_NEAR(test_obj.vert_arr[0].y, -1, 0.00001);
  ASSERT_NEAR(test_obj.vert_arr[0].z, -1, 0.00001);
  test.Scale(2);
  test_obj = test.GetObj();
  ASSERT_NEAR(test_obj.vert_arr[0].x, 2, 0.00001);
  ASSERT_NEAR(test_obj.vert_arr[0].y, -2, 0.00001);
  ASSERT_NEAR(test_obj.vert_arr[0].z, -2, 0.00001);
}

TEST(Viewer_v2, translate) {
  s21::ViewerModel test;
  test.LoadObj("tests/models/cube.obj");
  s21::ViewerModel::Obj test_obj = test.GetObj();
  ASSERT_NEAR(test_obj.vert_arr[0].x, 1, 0.00001);
  ASSERT_NEAR(test_obj.vert_arr[0].y, -1, 0.00001);
  ASSERT_NEAR(test_obj.vert_arr[0].z, -1, 0.00001);
  test.Translate(1, 1, 1);
  test_obj = test.GetObj();
  ASSERT_NEAR(test_obj.vert_arr[0].x, 2, 0.00001);
  ASSERT_NEAR(test_obj.vert_arr[0].y, 0, 0.00001);
  ASSERT_NEAR(test_obj.vert_arr[0].z, 0, 0.00001);
}

TEST(Viewer_v2, obj_transformation) {
  s21::ViewerModel test;
  test.LoadObj("tests/models/cube.obj");
  s21::ViewerModel::Transformations trans = test.GetObjCurrentTransformations();
  ASSERT_EQ(trans.pos[0], 0);
  ASSERT_EQ(trans.pos[1], 0);
  ASSERT_EQ(trans.pos[2], 0);
  ASSERT_EQ(trans.angle[0], 0);
  ASSERT_EQ(trans.angle[0], 0);
  ASSERT_EQ(trans.angle[0], 0);
  ASSERT_EQ(trans.scale, 1);
  test.SetObjCurrentTransformations(1, 2, 0.44, 1234, -24.6, 1e2, 2e-2);
  trans = test.GetObjCurrentTransformations();
  ASSERT_EQ(trans.pos[0], 1);
  ASSERT_EQ(trans.pos[1], 2);
  ASSERT_EQ(trans.pos[2], 0.44);
  ASSERT_EQ(trans.angle[0], 1234);
  ASSERT_EQ(trans.angle[1], -24.6);
  ASSERT_EQ(trans.angle[2], 1e2);
  ASSERT_EQ(trans.scale, 2e-2);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
