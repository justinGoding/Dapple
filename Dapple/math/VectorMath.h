#pragma once

#include "sfm.h"

static float angle(sfm::vec3f& a, sfm::vec3f& b);

static sfm::vec3f reflect(sfm::vec3f& R, sfm::vec3f& N);

static sfm::vec3f refract(sfm::vec3f& R, sfm::vec3f& N, int eta);