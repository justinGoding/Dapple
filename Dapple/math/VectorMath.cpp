#include <cmath>

#include "VectorMath.h"

float angle(sfm::vec3f& a, sfm::vec3f& b)
{
	return acos(
		sfm::dot(a, b) 
		/ 
		(sfm::magnitude(a) * sfm::magnitude(b))
		);
}

sfm::vec3f reflect(sfm::vec3f& R, sfm::vec3f& N)
{
	return R - (sfm::dot(2 * N, R)) * N;
}

sfm::vec3f refract(sfm::vec3f& R, sfm::vec3f& N, int eta)
{
	float NDotR = sfm::dot(N, R);
	float k = 1 - eta * eta * (1 - (NDotR * NDotR));

	if (k < 0.0f) return sfm::vec3f(0.0f);

	return eta * R - (eta * NDotR + std::sqrt(k)) * N;
}
