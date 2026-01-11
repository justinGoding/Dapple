#include "MatrixMath.h"

#include <cmath>

sfm::mat4f translation(float x, float y, float z)
{
	return sfm::mat4f(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x, y, z, 1.0f
	);
}

sfm::mat4f translation(const sfm::vec3f& t)
{
	return sfm::mat4f(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		t.x(), t.y(), t.z(), 1.0f
	);
}

sfm::mat4f rotation(float angle_x, float angle_y, float angle_z)
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

sfm::mat4f rotation(float angle, float x, float y, float z)
{
	const float x2 = x * x;
	const float y2 = y * y;
	const float z2 = z * z;
	const float c = cosf(angle);
	const float s = sinf(angle);
	const float omc = 1.0f - c;

	return sfm::mat4f(
		x2 * omc + c,			y * x * omc + z * s,	x * z * omc - y * s,	0,
		x * y * omc - z * s,	y2 * omc + c,			y * z * omc + x * s,	0,
		x * z * omc + y * s,	y * z * omc - x * s,	z2 * omc + c,			0,
		0,						0,						0,						1
	);
}

sfm::mat4f rotation(float angle, const sfm::vec3f& axis)
{
	return rotation(angle, axis.x(), axis.y(), axis.z());
}

sfm::mat4f scale(float x, float y, float z)
{
	return sfm::mat4f(
		x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

sfm::mat4f scale(const sfm::vec3f& s)
{
	return sfm::mat4f(
		s.x(), 0.0f, 0.0f, 0.0f,
		0.0f, s.y(), 0.0f, 0.0f,
		0.0f, 0.0f, s.z(), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

sfm::mat4f scale(float x)
{
	return sfm::mat4f(
		x, 0.0f, 0.0f, 0.0f,
		0.0f, x, 0.0f, 0.0f,
		0.0f, 0.0f, x, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

sfm::mat4f frustum(float left, float right, float bottom, float top, float near, float far)
{
	return sfm::mat4f(
		(2 * near) / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
		0.0f, (2 * near) / top - bottom, (top + bottom) / (top - bottom), 0.0f,
		0.0f, 0.0f, (near + far) / (near - far), (2 * near) / (near - far),
		0.0f, 0.0f, -1.0f, 0.0f
	);
}

sfm::mat4f perspective(float fovy, float aspect, float near, float far)
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

sfm::mat4f ortho(float left, float right, float bottom, float top, float near, float far)
{
	return sfm::mat4f(
		2 / (right - left), 0.0f,				0.0f,				(left + right) / (left - right),
		0.0f,				2 / (top - bottom), 0.0f,				(bottom + top) / (bottom - top),
		0.0f,				0.0f,				2 / (near - far),	(near + far) / (far - near),
		0.0f,				0.0f,				0.0f,				1.0f
	);
}

sfm::mat4f lookat(const sfm::vec3f& eye, const sfm::vec3f& center, const sfm::vec3f& up)
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