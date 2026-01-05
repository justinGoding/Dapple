#include <cmath>

#include "MatrixMath.h"

inline sfm::mat4f translation(float x, float y, float z)
{
	return sfm::mat4f(
		1.0f, 0.0f, 0.0f, x,
		0.0f, 1.0f, 0.0f, y,
		0.0f, 0.0f, 1.0f, z,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

inline sfm::mat4f translation(const sfm::vec3f& t)
{
	return sfm::mat4f(
		1.0f, 0.0f, 0.0f, t.x(),
		0.0f, 1.0f, 0.0f, t.y(),
		0.0f, 0.0f, 1.0f, t.z(),
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

inline sfm::mat4f rotation(float angle_x, float angle_y, float angle_z)
{
	float cos_x = cos(angle_x);
	float cos_y = cos(angle_y);
	float cos_z = cos(angle_z);

	float sin_x = sin(angle_x);
	float sin_y = sin(angle_y);
	float sin_z = sin(angle_z);

	return sfm::mat4f(
		cos_y * cos_z, cos_x * sin_z + sin_x * sin_y * cos_z, sin_x * sin_z - cos_x * sin_y * cos_z, 0.0f,
		-cos_y * sin_z, cos_x * cos_z - sin_x * sin_y * sin_z, sin_x * cos_z + cos_x * sin_y * sin_z, 0.0f,
		sin_y, -sin_x * cos_y, cos_x * cos_y, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

inline sfm::mat4f rotation(float angle, float x, float y, float z)
{
	float cos_t = cos(angle);
	float sin_t = sin(angle);

	return sfm::mat4f(
		x * x * (1 - cos_t) + cos_t, x * y * (1 - cos_t) + z * sin_t, x * z * (1 - cos_t) - y * sin_t, 0.0f,
		x * y * (1 - cos_t) - z * sin_t, y * y * (1 - cos_t) + cos_t, y * z * (1 - cos_t) + z * sin_t, 0.0f,
		x * z * (1 - cos_t) + y * sin_t, y * z * (1 - cos_t) - x * sin_t, z * z * (1 - cos_t) + cos_t, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

inline sfm::mat4f rotation(float angle, const sfm::vec3f& axis)
{
	float cos_t = cos(angle);
	float sin_t = sin(angle);

	return sfm::mat4f(
		axis.x() * axis.x() * (1 - cos_t) + cos_t, axis.x() * axis.y() * (1 - cos_t) + axis.z() * sin_t, axis.x() * axis.z() * (1 - cos_t) - axis.y() * sin_t, 0.0f,
		axis.x() * axis.y() * (1 - cos_t) - axis.z() * sin_t, axis.y() * axis.y() * (1 - cos_t) + cos_t, axis.y() * axis.z() * (1 - cos_t) + axis.z() * sin_t, 0.0f,
		axis.x() * axis.z() * (1 - cos_t) + axis.y() * sin_t, axis.y() * axis.z() * (1 - cos_t) - axis.x() * sin_t, axis.z() * axis.z() * (1 - cos_t) + cos_t, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

inline sfm::mat4f scale(float x, float y, float z)
{
	return sfm::mat4f(
		x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

inline sfm::mat4f scale(const sfm::vec3f& s)
{
	return sfm::mat4f(
		s.x(), 0.0f, 0.0f, 0.0f,
		0.0f, s.y(), 0.0f, 0.0f,
		0.0f, 0.0f, s.z(), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

inline sfm::mat4f scale(float x)
{
	return sfm::mat4f(
		x, 0.0f, 0.0f, 0.0f,
		0.0f, x, 0.0f, 0.0f,
		0.0f, 0.0f, x, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

inline sfm::mat4f frustum(float left, float right, float bottom, float top, float near, float far)
{
	return sfm::mat4f(
		(2 * near) / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
		0.0f, (2 * near) / top - bottom, (top + bottom) / (top - bottom), 0.0f,
		0.0f, 0.0f, (near + far) / (near - far), (2 * near) / (near - far),
		0.0f, 0.0f, -1.0f, 0.0f
	);
}

inline sfm::mat4f perspective(float fovy, float aspect, float near, float far)
{
	float q = 1.0f / tan(0.5f * fovy);
	float A = q / aspect;
	float B = (near + far) / (near - far);
	float C = (2.0f * near * far) / (near - far);

	return sfm::mat4f(
		A, 0.0f, 0.0f, 0.0f,
		0.0f, q, 0.0f, 0.0f,
		0.0f, 0.0f, B, -1.0f,
		0.0f, 0.0f, C, 0.0f
	);
}

inline sfm::mat4f ortho(float left, float right, float bottom, float top, float near, float far)
{
	return sfm::mat4f(
		2 / (right - left), 0.0f,				0.0f,				(left + right) / (left - right),
		0.0f,				2 / (top - bottom), 0.0f,				(bottom + top) / (bottom - top),
		0.0f,				0.0f,				2 / (near - far),	(near + far) / (far - near),
		0.0f,				0.0f,				0.0f,				1.0f
	);
}

inline sfm::mat4f lookat(const sfm::vec3f& eye, const sfm::vec3f& center, const sfm::vec3f& up)
{
	sfm::vec3f forward = sfm::normalize(center - eye);
	sfm::vec3f side = sfm::cross(forward, up);
	sfm::vec3f new_up = sfm::cross(side, forward);

	return sfm::mat4f(
		side.x(), new_up.x(), forward.x(), -eye.x(),
		side.y(), new_up.y(), forward.y(), -eye.y(),
		side.z(), new_up.z(), forward.z(), -eye.z(),
		0.0f,	  0.0f,		  0.0f,			1.0f
	);
}