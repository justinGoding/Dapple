#pragma once

#include "core.h"

#include <emmintrin.h>

// Shuffle helpers.
// Examples: SHUFFLE3(v, 0,1,2) leaves the vector unchanged.
//			 SHUFFLE3(v, 0,0,0) splats the X coord out.
#define SHUFFLE2F(V, X,Y) vec2f(_mm_shuffle_ps((V).m, (V).m, _MM_SHUFFLE(Y,Y,Y,X)))
#define SHUFFLE3F(V, X,Y,Z) vec3f(_mm_shuffle_ps((V).m, (V).m, _MM_SHUFFLE(Z,Z,Y,X)))
#define SHUFFLE4F(V, X,Y,Z,W) vec4f(_mm_shuffle_ps((V).m, (V).m, _MM_SHUFFLE(W,Z,Y,X)))

namespace sfm {

	struct vec4f
	{
		// Constructors
		VM_INLINE vec4f() : vec4f(0.0f) {}
		VM_INLINE explicit vec4f(float v) { m = _mm_set1_ps(v); }
		VM_INLINE explicit vec4f(int v) { m = _mm_set1_ps((float)v); }
		VM_INLINE explicit vec4f(double v) { m = _mm_set1_ps((float)v); }
		VM_INLINE explicit vec4f(const float* p) { m = _mm_set_ps(p[3], p[2], p[1], p[0]); }
		VM_INLINE explicit vec4f(float x, float y, float z, float w) { m = _mm_set_ps(w, z, y, x); }
		VM_INLINE explicit vec4f(__m128 v) { m = v; }

		VM_INLINE float x() const { return _mm_cvtss_f32(m); }
		VM_INLINE float y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
		VM_INLINE float z() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }
		VM_INLINE float w() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3))); }

		VM_INLINE vec4f yzxw() const { return SHUFFLE4F(*this, 1, 2, 0, 3); }
		VM_INLINE vec4f zxyw() const { return SHUFFLE4F(*this, 2, 0, 1, 3); }
		VM_INLINE vec4f wzyx() const { return SHUFFLE4F(*this, 3, 2, 1, 0); }

		VM_INLINE void store(float* p) const { p[0] = x(); p[1] = y(); p[2] = z(); p[3] = w(); }

		void setX(float x)
		{
			m = _mm_move_ss(m, _mm_set_ss(x));
		}

		void setY(float y)
		{
			__m128 t = _mm_move_ss(m, _mm_set_ss(y));
			t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
			m = _mm_move_ss(t, m);
		}

		void setZ(float z)
		{
			__m128 t = _mm_move_ss(m, _mm_set_ss(z));
			t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 1, 0));
			m = _mm_move_ss(t, m);
		}

		void setW(float w)
		{
			__m128 t = _mm_move_ss(m, _mm_set_ss(w));
			t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(0, 2, 1, 0));
			m = _mm_move_ss(t, m);
		}

		// Investigate slowness of this
		VM_INLINE float operator[] (size_t i) const { return m.m128_f32[i]; }
		VM_INLINE float& operator[] (size_t i) { return m.m128_f32[i]; }

		VM_INLINE operator const float* () const 
		{ 
			float p[4] = {0, 0, 0, 0};
			store(p);
			return p; 
		}

		__m128 m;
	};

	VM_INLINE vec4f vec4fi(int x, int y, int z, int w) { return vec4f((float)x, (float)y, (float)z, (float)w); }

#define VCONST extern const __declspec(selectany)
	struct vconstu
	{
		union { uint32_t u[4]; __m128 v; };
		VM_INLINE operator __m128() const { return v; }
		VM_INLINE operator __m128i() const { return _mm_castps_si128(v); }
	};

	VCONST vconstu vsignbits = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };

	typedef vec4f vec4b;

	VM_INLINE vec4f operator+ (vec4f a, vec4f b) { a.m = _mm_add_ps(a.m, b.m); return a; }
	VM_INLINE vec4f operator- (vec4f a, vec4f b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
	VM_INLINE vec4f operator* (vec4f a, vec4f b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
	VM_INLINE vec4f operator/ (vec4f a, vec4f b) { a.m = _mm_div_ps(a.m, b.m); return a; }
	VM_INLINE vec4f operator* (vec4f a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
	VM_INLINE vec4f operator/ (vec4f a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }
	VM_INLINE vec4f operator* (float a, vec4f b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }
	VM_INLINE vec4f operator/ (float a, vec4f b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }
	VM_INLINE vec4f& operator+= (vec4f& a, vec4f b) { a = a + b; return a; }
	VM_INLINE vec4f& operator-= (vec4f& a, vec4f b) { a = a - b; return a; }
	VM_INLINE vec4f& operator*= (vec4f& a, vec4f b) { a = a * b; return a; }
	VM_INLINE vec4f& operator/= (vec4f& a, vec4f b) { a = a / b; return a; }
	VM_INLINE vec4f& operator*= (vec4f& a, float b) { a = a * b; return a; }
	VM_INLINE vec4f& operator/= (vec4f& a, float b) { a = a / b; return a; }
	VM_INLINE vec4b operator== (vec4f a, vec4f b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
	VM_INLINE vec4b operator!= (vec4f a, vec4f b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
	VM_INLINE vec4b operator< (vec4f a, vec4f b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
	VM_INLINE vec4b operator> (vec4f a, vec4f b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }
	VM_INLINE vec4b operator<= (vec4f a, vec4f b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
	VM_INLINE vec4b operator>= (vec4f a, vec4f b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
	VM_INLINE vec4f min(vec4f a, vec4f b) { a.m = _mm_min_ps(a.m, b.m); return a; }
	VM_INLINE vec4f max(vec4f a, vec4f b) { a.m = _mm_max_ps(a.m, b.m); return a; }
	VM_INLINE vec4f operator- (vec4f a) { return vec4f(_mm_setzero_ps()) - a; }
	VM_INLINE vec4f abs(vec4f v) { v.m = _mm_andnot_ps(vsignbits, v.m); return v; }

	VM_INLINE float hmin(vec4f v)
	{
		v = min(v, SHUFFLE4F(v, 1, 0, 3, 2));
		return min(v, SHUFFLE4F(v, 3, 2, 1, 0)).x();
	}

	VM_INLINE float hmax(vec4f v)
	{
		v = max(v, SHUFFLE4F(v, 1, 0, 3, 2));
		return max(v, SHUFFLE4F(v, 3, 2, 1, 0)).x();
	}

	// Returns a 4-bit code where bit0..bit3 is X..W
	VM_INLINE unsigned mask(vec4f v) { return _mm_movemask_ps(v.m) & 15; }

	// Once we have a comparison, we can branch based on its results:
	VM_INLINE bool any(vec4b v) { return mask(v) != 0; }
	VM_INLINE bool all(vec4b v) { return mask(v) == 15; }

	VM_INLINE vec4f clamp(vec4f t, vec4f a, vec4f b) { return min(max(t, a), b); }
	VM_INLINE float sum(vec4f v) { return v.x() + v.y() + v.z() + v.w(); }
	VM_INLINE float dot(vec4f a, vec4f b) { return sum(a * b); }
	VM_INLINE float magnitude(vec4f v) { return sqrtf(dot(v, v)); }
	VM_INLINE float magnitudeSq(vec4f v) { return dot(v, v); }
	VM_INLINE vec4f normalize(vec4f v) { return v * (1.0f / magnitude(v)); }
	VM_INLINE vec4f lerp(vec4f a, vec4f b, float t) { return a + (b - a) * t; }

	struct vec3f
	{
		// Constructors
		VM_INLINE vec3f() : vec3f(0.0f) {}
		VM_INLINE explicit vec3f(float v) { m = _mm_set1_ps(v); }
		VM_INLINE explicit vec3f(int v) { m = _mm_set1_ps((float)v); }
		VM_INLINE explicit vec3f(double v) { m = _mm_set1_ps((float)v); }
		VM_INLINE explicit vec3f(const float* p) { m = _mm_set_ps(p[2], p[2], p[1], p[0]); }
		VM_INLINE explicit vec3f(float x, float y, float z) { m = _mm_set_ps(z, z, y, x); }
		VM_INLINE explicit vec3f(__m128 v) { m = v; }

		VM_INLINE float x() const { return _mm_cvtss_f32(m); }
		VM_INLINE float y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
		VM_INLINE float z() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }

		VM_INLINE vec3f yzx() const { return SHUFFLE3F(*this, 1, 2, 0); }
		VM_INLINE vec3f zxy() const { return SHUFFLE3F(*this, 2, 0, 1); }

		VM_INLINE void store(float* p) const { p[0] = x(); p[1] = y(); p[2] = z(); }

		void setX(float x)
		{
			m = _mm_move_ss(m, _mm_set_ss(x));
		}
		void setY(float y)
		{
			__m128 t = _mm_move_ss(m, _mm_set_ss(y));
			t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
			m = _mm_move_ss(t, m);
		}
		void setZ(float z)
		{
			__m128 t = _mm_move_ss(m, _mm_set_ss(z));
			t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 1, 0));
			m = _mm_move_ss(t, m);
		}

		VM_INLINE float operator[] (size_t i) const { return m.m128_f32[i]; };
		VM_INLINE float& operator[] (size_t i) { return m.m128_f32[i]; };

		__m128 m;
	};

	VM_INLINE vec3f vec3fi(int x, int y, int z) { return vec3f((float)x, (float)y, (float)z); }

	typedef vec3f vec3b;

	VM_INLINE vec3f operator+ (vec3f a, vec3f b) { a.m = _mm_add_ps(a.m, b.m); return a; }
	VM_INLINE vec3f operator- (vec3f a, vec3f b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
	VM_INLINE vec3f operator* (vec3f a, vec3f b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
	VM_INLINE vec3f operator/ (vec3f a, vec3f b) { a.m = _mm_div_ps(a.m, b.m); return a; }
	VM_INLINE vec3f operator* (vec3f a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
	VM_INLINE vec3f operator/ (vec3f a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }
	VM_INLINE vec3f operator* (float a, vec3f b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }
	VM_INLINE vec3f operator/ (float a, vec3f b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }
	VM_INLINE vec3f& operator+= (vec3f& a, vec3f b) { a = a + b; return a; }
	VM_INLINE vec3f& operator-= (vec3f& a, vec3f b) { a = a - b; return a; }
	VM_INLINE vec3f& operator*= (vec3f& a, vec3f b) { a = a * b; return a; }
	VM_INLINE vec3f& operator/= (vec3f& a, vec3f b) { a = a / b; return a; }
	VM_INLINE vec3f& operator*= (vec3f& a, float b) { a = a * b; return a; }
	VM_INLINE vec3f& operator/= (vec3f& a, float b) { a = a / b; return a; }
	VM_INLINE vec3b operator== (vec3f a, vec3f b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
	VM_INLINE vec3b operator!= (vec3f a, vec3f b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
	VM_INLINE vec3b operator< (vec3f a, vec3f b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
	VM_INLINE vec3b operator> (vec3f a, vec3f b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }
	VM_INLINE vec3b operator<= (vec3f a, vec3f b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
	VM_INLINE vec3b operator>= (vec3f a, vec3f b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
	VM_INLINE vec3f min(vec3f a, vec3f b) { a.m = _mm_min_ps(a.m, b.m); return a; }
	VM_INLINE vec3f max(vec3f a, vec3f b) { a.m = _mm_max_ps(a.m, b.m); return a; }
	VM_INLINE vec3f operator- (vec3f a) { return vec3f(_mm_setzero_ps()) - a; }
	VM_INLINE vec3f abs(vec3f v) { v.m = _mm_andnot_ps(vsignbits, v.m); return v; }

	VM_INLINE float hmin(vec3f v)
	{
		v = min(v, SHUFFLE3F(v, 1, 0, 2));
		return min(v, SHUFFLE3F(v, 2, 0, 1)).x();
	}

	VM_INLINE float hmax(vec3f v)
	{
		v = max(v, SHUFFLE3F(v, 1, 0, 2));
		return max(v, SHUFFLE3F(v, 2, 0, 1)).x();
	}

	VM_INLINE vec3f cross(vec3f a, vec3f b)
	{
		return (a.zxy() * b - a * b.zxy()).zxy();
	}

	// Returns a 3-bit code where bit0..bit2 is X..Z
	VM_INLINE unsigned mask(vec3f v) { return _mm_movemask_ps(v.m) & 7; }

	// Once we have a comparison, we can branch based on its result:
	VM_INLINE bool any(vec3b v) { return mask(v) != 0; }
	VM_INLINE bool all(vec3b v) { return mask(v) == 7; }

	VM_INLINE vec3f clamp(vec3f t, vec3f a, vec3f b) { return min(max(t, a), b); }
	VM_INLINE float sum(vec3f v) { return v.x() + v.y() + v.z(); }
	VM_INLINE float dot(vec3f a, vec3f b) { return sum(a * b); }
	VM_INLINE float magnitude(vec3f v) { return sqrtf(dot(v, v)); }
	VM_INLINE float magnitudeSq(vec3f v) { return dot(v, v); }
	VM_INLINE vec3f normalize(vec3f v) { return v * (1.0f / magnitude(v)); }
	VM_INLINE vec3f lerp(vec3f a, vec3f b, float t) { return a + (b - a) * t; }

	struct vec2f
	{
		// Constructors
		VM_INLINE vec2f() : vec2f(0.0f) {}
		VM_INLINE explicit vec2f(float v) { m = _mm_set1_ps(v); }
		VM_INLINE explicit vec2f(int v) { m = _mm_set1_ps((float)v); }
		VM_INLINE explicit vec2f(double v) { m = _mm_set1_ps((float)v); }
		VM_INLINE explicit vec2f(const float* p) { m = _mm_set_ps(p[1], p[1], p[1], p[0]); }
		VM_INLINE explicit vec2f(float x, float y) { m = _mm_set_ps(y, y, y, x); }
		VM_INLINE explicit vec2f(__m128 v) { m = v; }

		VM_INLINE float x() const { return _mm_cvtss_f32(m); }
		VM_INLINE float y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }

		VM_INLINE vec2f yx() const { return SHUFFLE2F(*this, 1, 0); }

		VM_INLINE void store(float* p) const { p[0] = x(); p[1] = y(); }

		void setX(float x)
		{
			m = _mm_move_ss(m, _mm_set_ss(x));
		}

		void setY(float y)
		{
			__m128 t = _mm_move_ss(m, _mm_set_ss(y));
			t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
			m = _mm_move_ss(t, m);
		}

		VM_INLINE float operator[] (size_t i) const { return m.m128_f32[i]; };
		VM_INLINE float& operator[] (size_t i) { return m.m128_f32[i]; };

		__m128 m;
	};

	VM_INLINE vec2f vec2fi(int x, int y) { return vec2f((float)x, (float)y); }

	typedef vec2f vec2b;

	VM_INLINE vec2f operator+ (vec2f a, vec2f b) { a.m = _mm_add_ps(a.m, b.m); return a; }
	VM_INLINE vec2f operator- (vec2f a, vec2f b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
	VM_INLINE vec2f operator* (vec2f a, vec2f b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
	VM_INLINE vec2f operator/ (vec2f a, vec2f b) { a.m = _mm_div_ps(a.m, b.m); return a; }
	VM_INLINE vec2f operator* (vec2f a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
	VM_INLINE vec2f operator/ (vec2f a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }
	VM_INLINE vec2f operator* (float a, vec2f b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }
	VM_INLINE vec2f operator/ (float a, vec2f b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }
	VM_INLINE vec2f& operator+= (vec2f& a, vec2f b) { a = a + b; return a; }
	VM_INLINE vec2f& operator-= (vec2f& a, vec2f b) { a = a - b; return a; }
	VM_INLINE vec2f& operator*= (vec2f& a, vec2f b) { a = a * b; return a; }
	VM_INLINE vec2f& operator/= (vec2f& a, vec2f b) { a = a / b; return a; }
	VM_INLINE vec2f& operator*= (vec2f& a, float b) { a = a * b; return a; }
	VM_INLINE vec2f& operator/= (vec2f& a, float b) { a = a / b; return a; }
	VM_INLINE vec2b operator== (vec2f a, vec2f b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
	VM_INLINE vec2b operator!= (vec2f a, vec2f b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
	VM_INLINE vec2b operator< (vec2f a, vec2f b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
	VM_INLINE vec2b operator> (vec2f a, vec2f b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }
	VM_INLINE vec2b operator<= (vec2f a, vec2f b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
	VM_INLINE vec2b operator>= (vec2f a, vec2f b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
	VM_INLINE vec2f min(vec2f a, vec2f b) { a.m = _mm_min_ps(a.m, b.m); return a; }
	VM_INLINE vec2f max(vec2f a, vec2f b) { a.m = _mm_max_ps(a.m, b.m); return a; }

	VM_INLINE vec2f operator- (vec2f a) { return vec2f(_mm_setzero_ps()) - a; }
	VM_INLINE vec2f abs(vec2f v) { v.m = _mm_andnot_ps(vsignbits, v.m); return v; }

	VM_INLINE float hmin(vec2f v)
	{
		return min(v, SHUFFLE2F(v, 1, 0)).x();
	}

	VM_INLINE float hmax(vec2f v)
	{
		return max(v, SHUFFLE2F(v, 1, 0)).x();
	}

	VM_INLINE float cross(vec2f a, vec2f b)
	{
		a = a * b.yx();
		return a.x() - a.y();
	}

	// Returns a 3-bit code where bit0..bit2 is X..Z
	VM_INLINE unsigned mask(vec2f v) { return _mm_movemask_ps(v.m) & 3; }

	// Once we have a comparison, we can branch based on its result:
	VM_INLINE bool any(vec2b v) { return mask(v) != 0; }
	VM_INLINE bool all(vec2b v) { return mask(v) == 3; }

	VM_INLINE vec2f clamp(vec2f t, vec2f a, vec2f b) { return min(max(t, a), b); }
	VM_INLINE float sum(vec2f v) { return v.x() + v.y(); }
	VM_INLINE float dot(vec2f a, vec2f b) { return sum(a * b); }
	VM_INLINE float magnitude(vec2f v) { return sqrtf(dot(v, v)); }
	VM_INLINE float magnitudeSq(vec2f v) { return dot(v, v); }
	VM_INLINE vec2f normalize(vec2f v) { return v * (1.0f / magnitude(v)); }
	VM_INLINE vec2f lerp(vec2f a, vec2f b, float t) { return a + (b - a) * t; }
}