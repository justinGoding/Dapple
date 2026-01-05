#pragma once

#include "core.h"
#include "vector.h"

namespace sfm
{
	struct Quaternion
	{
	public:
		Quaternion()
		{
			n = 0;
			v = vec3f(0.0f);
		}
		Quaternion(float e0, float e1, float e2, float e3)
		{
			n = e0;
			v = vec3f(e1, e2, e3);
		}
		Quaternion(float q0, vec3f qv)
		{
			n = q0;
			v = qv;
		}

		float n;
		vec3f v;
	};

	VM_INLINE Quaternion operator~ (Quaternion q) { return Quaternion(q.n, -q.v); }

	VM_INLINE Quaternion operator+ (const Quaternion& q, const Quaternion& p)
	{
		return Quaternion(q.n + p.n, q.v + p.v);
	}

	VM_INLINE Quaternion operator- (const Quaternion& q, const Quaternion& p)
	{
		return Quaternion(q.n - p.n, q.v - p.v);
	}

	VM_INLINE Quaternion operator* (const Quaternion& q, float s)
	{
		return Quaternion(q.n * s, q.v * s);
	}
	VM_INLINE Quaternion operator* (float s, const Quaternion& q)
	{
		return Quaternion(q.n * s, q.v * s);
	}

	VM_INLINE Quaternion operator* (const Quaternion& q, const Quaternion& p)
	{
		return Quaternion(q.n * p.n - dot(q.v, p.v), q.n * p.v + p.n * q.v + cross(q.v, p.v));
	}
	VM_INLINE Quaternion operator* (const Quaternion& q, const vec3f& v)
	{
		return Quaternion(-dot(q.v, v), q.n * v + cross(q.v, v));
	}
	VM_INLINE Quaternion operator* (const vec3f& v, const Quaternion& q)
	{
		return Quaternion(-dot(q.v, v), q.n * v + cross(q.v, v));
	}

	VM_INLINE Quaternion operator/ (const Quaternion& q, float s)
	{
		return Quaternion(q.n / s, q.v / s);
	}

	VM_INLINE Quaternion& operator+= (Quaternion& q, const Quaternion& p)
	{
		q.n += p.n;
		q.v += p.v;
		return q;
	}

	VM_INLINE Quaternion& operator-= (Quaternion& q, const Quaternion& p)
	{
		q.n -= p.n;
		q.v -= p.v;
		return q;
	}

	VM_INLINE Quaternion& operator*= (Quaternion& q, float s)
	{
		q.n *= s;
		q.v *= s;
		return q;
	}

	VM_INLINE Quaternion& operator/= (Quaternion& q, float s)
	{
		q.n /= s;
		q.v /= s;
		return q;
	}

	VM_INLINE float magnitude(Quaternion q)
	{
		return magnitude(vec4f(q.n, q.v.x(), q.v.y(), q.v.z()));
	}

	VM_INLINE Quaternion QRotate(const Quaternion& q, const Quaternion& p)
	{
		return q * p * ~q;
	}

	VM_INLINE vec3f QVRotate(const Quaternion& q, const vec3f& v)
	{
		return (q * v * ~q).v;
	}

	VM_INLINE Quaternion QuaternionFromAngleAndAxis(float t, const vec3f& n)
	{
		return Quaternion(cos(t / 2), sin(t / 2) * n);
	}

	VM_INLINE Quaternion QuaternionFromAngleAndAxis(float t, float x, float y, float z)
	{
		return Quaternion(cos(t / 2), sin(t / 2) * vec3f(x, y, z));
	}

	VM_INLINE Quaternion QuaternionFromEulerAngles(float x, float y, float z)
	{
		Quaternion q_roll = Quaternion(cos(x / 2), vec3f(sin(x / 2), 0.0f, 0.0f));
		Quaternion q_pitch = Quaternion(cos(y / 2), vec3f(0.0f, sin(y / 2), 0.0f));
		Quaternion q_yaw = Quaternion(cos(z / 2), vec3f(0.0f, 0.0f, sin(z / 2)));

		return q_roll * q_pitch * q_yaw;
	}

	VM_INLINE vec3f EulerAnglesFromQuaternion(Quaternion q)
	{
		float q00 = q.n * q.n;
		float q11 = q.v.x() * q.v.x();
		float q22 = q.v.y() * q.v.y();
		float q33 = q.v.z() * q.v.z();

		float r11 = q00 + q11 - q22 - q33;
		float r21 = 2 * (q.v.x() * q.v.y() + q.n * q.v.z());
		float r31 = 2 * (q.v.x() * q.v.z() - q.n * q.v.y());
		float r32 = 2 * (q.v.y() * q.v.z() + q.n * q.v.x());
		float r33 = q00 - q11 - q22 + q33;

		float tmp = fabs(r31);
		if (tmp > 0.999999f)
		{
			float r12 = 2 * (q.v.x() * q.v.y() - q.n * q.v.z());
			float r13 = 2 * (q.v.x() * q.v.z() + q.n * q.v.y());

			return vec3f(0.0f, -(M_PI / 2) * r31 / tmp, atan2(-r12, -r31 * r13));
		}

		return vec3f(atan2(r32, r33), asin(-r31), atan2(r21, r11));
	}

	VM_INLINE float GetAngle(const Quaternion& q)
	{
		return (float)(2 * acos(q.n));
	}

	VM_INLINE vec3f GetAxis(const Quaternion& q)
	{
		return q.v / magnitude(q);
	}
}