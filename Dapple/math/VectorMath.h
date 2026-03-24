#pragma once

#include "sfm.h"

float angle(sfm::vec3f& a, sfm::vec3f& b);

sfm::vec3f reflect(sfm::vec3f& R, sfm::vec3f& N);

sfm::vec3f refract(sfm::vec3f& R, sfm::vec3f& N, int eta);