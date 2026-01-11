#pragma once

#include "vector.h"

namespace sfm
{
	/// //////////////////////////////////////////
	/// 4 x 4 Matrix
	/// /////////////////////////////////////////

	struct mat4f
	{
		// Constructors
		VM_INLINE mat4f() : mat4f(vec4f(1, 0, 0, 0), vec4f(0, 1, 0, 0), vec4f(0, 0, 1, 0), vec4f(0, 0, 0, 1)) {}
		VM_INLINE mat4f(float v) 
		{ 
			rows[0] = vec4f(v); 
			rows[1] = vec4f(v);
			rows[2] = vec4f(v);
			rows[3] = vec4f(v);
		}
		VM_INLINE mat4f(vec4f a, vec4f b, vec4f c, vec4f d)
		{
			rows[0] = a;
			rows[1] = b;
			rows[2] = c;
			rows[3] = d;
		}
		VM_INLINE mat4f(float a1, float a2, float a3, float a4,
			float b1, float b2, float b3, float b4,
			float c1, float c2, float c3, float c4,
			float d1, float d2, float d3, float d4)
		{
			rows[0] = vec4f(a1, a2, a3, a4);
			rows[1] = vec4f(b1, b2, b3, b4);
			rows[2] = vec4f(c1, c2, c3, c4);
			rows[3] = vec4f(d1, d2, d3, d4);
		}

		VM_INLINE static mat4f identity()
		{
			return mat4f();
		}

		vec4f& operator[] (int index)
		{
			return rows[index];
		}
		const vec4f& operator[] (int index) const
		{
			return rows[index];
		}

		VM_INLINE operator const float* () const { return (float*) & rows; }

		vec4f rows[4];
	};

	VM_INLINE mat4f transpose(const mat4f& m)
	{
		mat4f r = mat4f(0.0f);
		r.rows[0] = vec4f(m.rows[0].x(), m.rows[1].x(), m.rows[2].x(), m.rows[3].x());
		r.rows[1] = vec4f(m.rows[0].y(), m.rows[1].y(), m.rows[2].y(), m.rows[3].y());
		r.rows[2] = vec4f(m.rows[0].z(), m.rows[1].z(), m.rows[2].z(), m.rows[3].z());
		r.rows[3] = vec4f(m.rows[0].w(), m.rows[1].w(), m.rows[2].w(), m.rows[3].w());
		return r;
	}

	VM_INLINE mat4f operator+ (const mat4f& a, const mat4f& b) 
	{
		mat4f r = mat4f(0.0f);
		r.rows[0] = a.rows[0] + b.rows[0];
		r.rows[1] = a.rows[1] + b.rows[1];
		r.rows[2] = a.rows[2] + b.rows[2];
		r.rows[3] = a.rows[3] + b.rows[3];
		return r;
	}

	VM_INLINE mat4f operator- (const mat4f& a, const mat4f& b)
	{
		mat4f r = mat4f(0.0f);
		r.rows[0] = a.rows[0] - b.rows[0];
		r.rows[1] = a.rows[1] - b.rows[1];
		r.rows[2] = a.rows[2] - b.rows[2];
		r.rows[3] = a.rows[3] - b.rows[3];
		return r;
	}

	VM_INLINE mat4f operator* (mat4f a, float b)
	{
		a.rows[0] = a.rows[0] * b;
		a.rows[1] = a.rows[1] * b;
		a.rows[2] = a.rows[2] * b;
		a.rows[3] = a.rows[3] * b;
		return a;
	}

	VM_INLINE mat4f operator* (float b, mat4f a)
	{
		a.rows[0] = a.rows[0] * b;
		a.rows[1] = a.rows[1] * b;
		a.rows[2] = a.rows[2] * b;
		a.rows[3] = a.rows[3] * b;
		return a;
	}

	VM_INLINE vec4f operator* (vec4f a, mat4f b)
	{
		b = transpose(b);
		return vec4f(dot(a, b.rows[0]), dot(a, b.rows[1]), dot(a, b.rows[2]), dot(a, b.rows[3]));
	}

	VM_INLINE vec4f operator* (mat4f b, vec4f a)
	{
		return vec4f(dot(a, b.rows[0]), dot(a, b.rows[1]), dot(a, b.rows[2]), dot(a, b.rows[3]));
	}

	VM_INLINE mat4f operator* (const mat4f& a, mat4f b)
	{
		mat4f r = mat4f(0.0f);
		b = transpose(b);
		r.rows[0].setX(dot(a.rows[0], b.rows[0]));
		r.rows[0].setY(dot(a.rows[0], b.rows[1]));
		r.rows[0].setZ(dot(a.rows[0], b.rows[2]));
		r.rows[0].setW(dot(a.rows[0], b.rows[3]));

		r.rows[1].setX(dot(a.rows[1], b.rows[0]));
		r.rows[1].setY(dot(a.rows[1], b.rows[1]));
		r.rows[1].setZ(dot(a.rows[1], b.rows[2]));
		r.rows[1].setW(dot(a.rows[1], b.rows[3]));

		r.rows[2].setX(dot(a.rows[2], b.rows[0]));
		r.rows[2].setY(dot(a.rows[2], b.rows[1]));
		r.rows[2].setZ(dot(a.rows[2], b.rows[2]));
		r.rows[2].setW(dot(a.rows[2], b.rows[3]));

		r.rows[3].setX(dot(a.rows[3], b.rows[0]));
		r.rows[3].setY(dot(a.rows[3], b.rows[1]));
		r.rows[3].setZ(dot(a.rows[3], b.rows[2]));
		r.rows[3].setW(dot(a.rows[3], b.rows[3]));

		return r;
	}

	VM_INLINE mat4f& operator+= (mat4f& a, const mat4f& b)
	{
		a = a + b;
		return a;
	}

	VM_INLINE mat4f& operator-= (mat4f& a, const mat4f& b)
	{
		a = a - b;
		return a;
	}

	VM_INLINE mat4f& operator*= (mat4f& a, float b)
	{
		a = a * b;
		return a;
	}

	VM_INLINE mat4f& operator*= (mat4f& a, const mat4f& b)
	{
		a = a * b;
		return a;
	}

	VM_INLINE mat4f operator- (mat4f m)
	{
		return mat4f(0.0f) - m;
	}

	VM_INLINE bool operator== (const mat4f& a, const mat4f& b)
	{
		return all(a[0] == b[0]) &&
			all(a[1] == b[1]) &&
			all(a[2] == b[2]) &&
			all(a[3] == b[3]);
	}

	VM_INLINE bool operator!= (const mat4f& a, const mat4f& b)
	{
		return !(a == b);
	}

	VM_INLINE mat4f abs(mat4f m)
	{
		m.rows[0] = abs(m.rows[0]);
		m.rows[1] = abs(m.rows[1]);
		m.rows[2] = abs(m.rows[2]);
		m.rows[3] = abs(m.rows[3]);
		return m;
	}

	VM_INLINE float det(const mat4f& m)
	{
		return	m.rows[0].x()
			* (
				m.rows[1].y() * (m.rows[2].z() * m.rows[3].w() - m.rows[2].w() * m.rows[3].z())
				+ m.rows[1].z() * (m.rows[2].w() * m.rows[3].y() - m.rows[2].y() * m.rows[3].w())
				+ m.rows[1].w() * (m.rows[2].y() * m.rows[3].z() - m.rows[2].z() * m.rows[3].y())
				)
			- m.rows[0].y()
			* (
				m.rows[1].x() * (m.rows[2].z() * m.rows[3].w() - m.rows[2].w() * m.rows[3].z())
				+ m.rows[1].z() * (m.rows[2].w() * m.rows[3].x() - m.rows[2].x() * m.rows[3].w())
				+ m.rows[1].w() * (m.rows[2].x() * m.rows[3].z() - m.rows[2].z() * m.rows[3].x())
				)
			+ m.rows[0].z()
			* (
				m.rows[1].x() * (m.rows[2].y() * m.rows[3].w() - m.rows[2].w() * m.rows[3].y())
				+ m.rows[1].y() * (m.rows[2].w() * m.rows[3].x() - m.rows[2].x() * m.rows[3].w())
				+ m.rows[1].w() * (m.rows[2].x() * m.rows[3].y() - m.rows[2].y() * m.rows[3].x())
				)
			- m.rows[0].w()
			* (
				m.rows[1].x() * (m.rows[2].y() * m.rows[3].z() - m.rows[2].z() * m.rows[3].y())
				+ m.rows[1].y() * (m.rows[2].z() * m.rows[3].x() - m.rows[2].x() * m.rows[3].z())
				+ m.rows[1].z() * (m.rows[2].x() * m.rows[3].y() - m.rows[2].y() * m.rows[3].x())
				);
	}

	/// //////////////////////////////////////////
	/// 3 x 3 Matrix
	/// /////////////////////////////////////////

	struct mat3f
	{
		// Constructors
		VM_INLINE mat3f() : mat3f(vec3f(1, 0, 0), vec3f(0, 1, 0), vec3f(0, 0, 1)) {}
		VM_INLINE mat3f(float v)
		{
			rows[0] = vec3f(v);
			rows[1] = vec3f(v);
			rows[2] = vec3f(v);
		}
		VM_INLINE mat3f(vec3f a, vec3f b, vec3f c)
		{
			rows[0] = a;
			rows[1] = b;
			rows[2] = c;
		}
		VM_INLINE mat3f(float a1, float a2, float a3,
			float b1, float b2, float b3,
			float c1, float c2, float c3)
		{
			rows[0] = vec3f(a1, a2, a3);
			rows[1] = vec3f(b1, b2, b3);
			rows[2] = vec3f(c1, c2, c3);
		}

		VM_INLINE static mat3f identity()
		{
			return mat3f();
		}

		vec3f& operator[] (int index)
		{
			return rows[index];
		}
		const vec3f& operator[] (int index) const
		{
			return rows[index];
		}

		vec3f rows[3];
	};

	VM_INLINE mat3f transpose(const mat3f& m)
	{
		mat3f r = mat3f(0.0f);
		r.rows[0] = vec3f(m.rows[0].x(), m.rows[1].x(), m.rows[2].x());
		r.rows[1] = vec3f(m.rows[0].y(), m.rows[1].y(), m.rows[2].y());
		r.rows[2] = vec3f(m.rows[0].z(), m.rows[1].z(), m.rows[2].z());
		return r;
	}

	VM_INLINE mat3f operator+ (const mat3f& a, const mat3f& b)
	{
		mat3f r = mat3f(0.0f);
		r.rows[0] = a.rows[0] + b.rows[0];
		r.rows[1] = a.rows[1] + b.rows[1];
		r.rows[2] = a.rows[2] + b.rows[2];
		return r;
	}

	VM_INLINE mat3f operator- (const mat3f& a, const mat3f& b)
	{
		mat3f r = mat3f(0.0f);
		r.rows[0] = a.rows[0] - b.rows[0];
		r.rows[1] = a.rows[1] - b.rows[1];
		r.rows[2] = a.rows[2] - b.rows[2];
		return r;
	}

	VM_INLINE mat3f operator* (mat3f a, float b)
	{
		a.rows[0] = a.rows[0] * b;
		a.rows[1] = a.rows[1] * b;
		a.rows[2] = a.rows[2] * b;
		return a;
	}

	VM_INLINE mat3f operator* (float b, mat3f a)
	{
		a.rows[0] = a.rows[0] * b;
		a.rows[1] = a.rows[1] * b;
		a.rows[2] = a.rows[2] * b;
		return a;
	}

	VM_INLINE vec3f operator* (vec3f a, mat3f b)
	{
		b = transpose(b);
		return vec3f(dot(a, b.rows[0]), dot(a, b.rows[1]), dot(a, b.rows[2]));
	}

	VM_INLINE vec3f operator* (mat3f b, vec3f a)
	{
		return vec3f(dot(a, b.rows[0]), dot(a, b.rows[1]), dot(a, b.rows[2]));
	}

	VM_INLINE mat3f operator* (const mat3f& a, mat3f b)
	{
		mat3f r = mat3f(0.0f);
		b = transpose(b);
		r.rows[0].setX(dot(a.rows[0], b.rows[0]));
		r.rows[0].setY(dot(a.rows[0], b.rows[1]));
		r.rows[0].setZ(dot(a.rows[0], b.rows[2]));

		r.rows[1].setX(dot(a.rows[1], b.rows[0]));
		r.rows[1].setY(dot(a.rows[1], b.rows[1]));
		r.rows[1].setZ(dot(a.rows[1], b.rows[2]));

		r.rows[2].setX(dot(a.rows[2], b.rows[0]));
		r.rows[2].setY(dot(a.rows[2], b.rows[1]));
		r.rows[2].setZ(dot(a.rows[2], b.rows[2]));

		return r;
	}

	VM_INLINE mat3f& operator+= (mat3f& a, const mat3f& b)
	{
		a = a + b;
		return a;
	}

	VM_INLINE mat3f& operator-= (mat3f& a, const mat3f& b)
	{
		a = a - b;
		return a;
	}

	VM_INLINE mat3f& operator*= (mat3f& a, float b)
	{
		a = a * b;
		return a;
	}

	VM_INLINE mat3f& operator*= (mat3f& a, const mat3f& b)
	{
		a = a * b;
		return a;
	}

	VM_INLINE mat3f operator- (mat3f m)
	{
		return mat3f(0.0f) - m;
	}

	VM_INLINE bool operator== (const mat3f& a, const mat3f& b)
	{
		return all(a[0] == b[0]) &&
			all(a[1] == b[1]) &&
			all(a[2] == b[2]);
	}

	VM_INLINE bool operator!= (const mat3f& a, const mat3f& b)
	{
		return !(a == b);
	}

	VM_INLINE mat3f abs(mat3f m)
	{
		m.rows[0] = abs(m.rows[0]);
		m.rows[1] = abs(m.rows[1]);
		m.rows[2] = abs(m.rows[2]);
		return m;
	}

	VM_INLINE float det(const mat3f& m)
	{
		return dot(cross(m[0], m[1]), m[2]);
	}

	/// //////////////////////////////////////////
	/// 2 x 2 Matrix
	/// /////////////////////////////////////////
	
	struct mat2f
	{
		// Constructors
		VM_INLINE mat2f() : mat2f(vec2f(1, 0), vec2f(0, 1)) {}
		VM_INLINE mat2f(float v)
		{
			rows[0] = vec2f(v);
			rows[1] = vec2f(v);
		}
		VM_INLINE mat2f(vec2f a, vec2f b)
		{
			rows[0] = a;
			rows[1] = b;
		}
		VM_INLINE mat2f(float a1, float a2,
			float b1, float b2)
		{
			rows[0] = vec2f(a1, a2);
			rows[1] = vec2f(b1, b2);
		}

		VM_INLINE static mat2f identity()
		{
			return mat2f();
		}

		vec2f& operator[] (int index)
		{
			return rows[index];
		}
		const vec2f& operator[] (int index) const
		{
			return rows[index];
		}

		vec2f rows[2];
	};

	VM_INLINE mat2f transpose(const mat2f& m)
	{
		mat2f r = mat2f(0.0f);
		r.rows[0] = vec2f(m.rows[0].x(), m.rows[1].x());
		r.rows[1] = vec2f(m.rows[0].y(), m.rows[1].y());
		return r;
	}

	VM_INLINE mat2f operator+ (const mat2f& a, const mat2f& b)
	{
		mat2f r = mat2f(0.0f);
		r.rows[0] = a.rows[0] + b.rows[0];
		r.rows[1] = a.rows[1] + b.rows[1];
		return r;
	}

	VM_INLINE mat2f operator- (const mat2f& a, const mat2f& b)
	{
		mat2f r = mat2f(0.0f);
		r.rows[0] = a.rows[0] - b.rows[0];
		r.rows[1] = a.rows[1] - b.rows[1];
		return r;
	}

	VM_INLINE mat2f operator* (mat2f a, float b)
	{
		a.rows[0] = a.rows[0] * b;
		a.rows[1] = a.rows[1] * b;
		return a;
	}

	VM_INLINE mat2f operator* (float b, mat2f a)
	{
		a.rows[0] = a.rows[0] * b;
		a.rows[1] = a.rows[1] * b;
		return a;
	}

	VM_INLINE vec2f operator* (const vec2f& a, mat2f b)
	{
		b = transpose(b);
		return vec2f(dot(a, b.rows[0]), dot(a, b.rows[1]));
	}

	VM_INLINE vec2f operator* (mat2f b, vec2f a)
	{
		return vec2f(dot(a, b.rows[0]), dot(a, b.rows[1]));
	}

	VM_INLINE mat2f operator* (const mat2f& a, mat2f b)
	{
		mat2f r = mat2f(0.0f);
		b = transpose(b);
		r.rows[0].setX(dot(a.rows[0], b.rows[0]));
		r.rows[0].setY(dot(a.rows[0], b.rows[1]));

		r.rows[1].setX(dot(a.rows[1], b.rows[0]));
		r.rows[1].setY(dot(a.rows[1], b.rows[1]));

		return r;
	}

	VM_INLINE mat2f& operator+= (mat2f& a, const mat2f& b)
	{
		a = a + b;
		return a;
	}

	VM_INLINE mat2f& operator-= (mat2f& a, const mat2f& b)
	{
		a = a - b;
		return a;
	}

	VM_INLINE mat2f& operator*= (mat2f& a, float b)
	{
		a = a * b;
		return a;
	}

	VM_INLINE mat2f& operator*= (mat2f& a, const mat2f& b)
	{
		a = a * b;
		return a;
	}

	VM_INLINE mat2f operator- (mat2f m)
	{
		return mat2f(0.0f) - m;
	}

	VM_INLINE bool operator== (const mat2f& a, const mat2f& b)
	{
		return all(a[0] == b[0]) &&
			all(a[1] == b[1]);
	}

	VM_INLINE bool operator!= (const mat2f& a, const mat2f& b)
	{
		return !(a == b);
	}

	VM_INLINE mat2f abs(mat2f m)
	{
		m.rows[0] = abs(m.rows[0]);
		m.rows[1] = abs(m.rows[1]);
		return m;
	}

	VM_INLINE float det(const mat2f& m)
	{
		return m[0].x() * m[1].y() - m[0].y() * m[1].x();
	}
}