#pragma once

#include "sfm.h"

sfm::mat4f translation(float x, float y, float z);

sfm::mat4f translation(const sfm::vec3f& t);

sfm::mat4f rotation(float angle_x, float angle_y, float angle_z);

sfm::mat4f rotation(float angle, float x, float y, float z);

sfm::mat4f rotation(float angle, const sfm::vec3f& axis);

sfm::mat4f scale(float x, float y, float z);

sfm::mat4f scale(const sfm::vec3f& s);

sfm::mat4f scale(float x);

sfm::mat4f frustum(float left, float right, float bottom, float top, float near, float far);

sfm::mat4f perspective(float fovy, float aspect, float near, float far);

sfm::mat4f ortho(float left, float right, float bottom, float top, float near, float far);

sfm::mat4f lookat(const sfm::vec3f& eye, const sfm::vec3f& center, const sfm::vec3f& up);