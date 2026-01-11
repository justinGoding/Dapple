#include "pch.h"
#include "CppUnitTest.h"
#include "sfm.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace sfmUnitTests
{
	using namespace sfm;

	TEST_CLASS(sfmVec4fUnitTests)
	{
	public:

		TEST_METHOD(Constructor1)
		{
			mat4f m = mat4f(0.0f);
			vec4f v = vec4f(0.0f);
			Assert::IsTrue(m[0] == v);
			Assert::IsTrue(m[1] == v);
			Assert::IsTrue(m[2] == v);
			Assert::IsTrue(m[3] == v);
		}

		TEST_METHOD(transpose1)
		{
			mat4f a = mat4f();
			mat4f b = transpose(a);
			Assert::IsTrue(a == b);
		}

		TEST_METHOD(transpose2)
		{
			mat4f a = mat4f(
				1.0f, 2.0f, 3.0f, 4.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				4.0f, 3.0f, 2.0f, 1.0f,
				5.0f, 6.0f, 7.0f, 8.0f
			);

			mat4f b = mat4f(
				1, 0, 4, 5,
				2, 0, 3, 6,
				3, 0, 2, 7,
				4, 0, 1, 8
			);
			Assert::IsTrue(transpose(a) == b);
		}
		TEST_METHOD(add1)
		{
			mat4f a = mat4f(1.0f);
			mat4f b = mat4f(2.0f);
			mat4f c = mat4f(3.0f);
			Assert::IsTrue(a + b == c);
		}

		TEST_METHOD(sub1)
		{
			mat4f a = mat4f(1.0f);
			mat4f b = mat4f(2.0f);
			mat4f c = mat4f(3.0f);
			Assert::IsTrue(c - b == a);
		}

		TEST_METHOD(multfloat1)
		{
			mat4f a = mat4f(1.0f);
			mat4f b = mat4f(4.0f);
			Assert::IsTrue(a * 4 == b);
		}

		TEST_METHOD(multfloat2)
		{
			mat4f a = mat4f(1.0f);
			mat4f b = mat4f(4.0f);
			Assert::IsTrue(4 * a == b);
		}

		TEST_METHOD(mult1)
		{
			mat2f a = mat2f(
				1, -2,
				5, 0
			);
			mat2f b = mat2f(
				-3, 7,
				4, 3
			);
			mat2f c = mat2f(
				-11, 1,
				-15, 35
			);
			Assert::IsTrue(a * b == c);
		}

		TEST_METHOD(mult2)
		{
			vec4f v = vec4f(1, 2, 3, 4);
			mat4f m = mat4f();
			Assert::IsTrue(v * m == v);
		}

		TEST_METHOD(mult3)
		{
			mat2f i = mat2f();
			mat2f a = mat2f(
				1, 2,
				3, 4
			);
			Assert::IsTrue(i * a == a);
		}

		TEST_METHOD(mult4)
		{
			vec2f v = vec2f(3, 3);
			mat2f m = mat2f(6, -7, -4, 5);
			vec2f r = vec2f(6, -6);
			vec2f vm = v * m;
			Assert::IsTrue(all(vm == r));
		}

		TEST_METHOD(mult5)
		{
			vec3f v = vec3f(5, -1, 2);
			mat3f m = mat3f();
			Assert::IsTrue(all(v * m == v));
		}

		TEST_METHOD(mult6)
		{
			vec3f v = vec3f(5, -1, 2);
			mat3f m = mat3f(
				2, 5, -3,
				1, 7, 1,
				-2, -1, 4
			);
			vec3f r = vec3f(5, 16, -8);
			vec3f vm = v * m;
			Assert::IsTrue(all(v * m == r));
		}

		TEST_METHOD(mult7)
		{
			vec3f v = vec3f(5, -1, 2);
			mat3f m = mat3f(
				1, 7, 2,
				7, 0, -3,
				2, -3, -1
			);
			vec3f r = vec3f(2, 29, 11);
			Assert::IsTrue(all(v * m == r));
		}

		TEST_METHOD(mult8)
		{
			vec3f v = vec3f(5, -1, 2);
			mat3f m = mat3f(
				0, -4, 3,
				4, 0, -1, 
				-3, 1, 0
			);
			vec3f r = vec3f(-10, -18, 16);
			Assert::IsTrue(all(v * m == r));
		}

		TEST_METHOD(mult9)
		{
			mat2f a = mat2f(
				0, -1,
				1, 0
			);
			mat2f b = mat2f(
				2, 0,
				0, 2
			);
			mat2f c = mat2f(0, -2, 2, 0);
			Assert::IsTrue(a * b == c);
		}

		TEST_METHOD(mult10)
		{
			vec3f v = vec3f(5, -1, 2);
			mat3f m = mat3f(
				2, 5, -3,
				1, 7, 1,
				-2, -1, 4
			);
			vec3f r = vec3f(-1, 0, -1);
			vec3f vm = v * m;
			Assert::IsTrue(all(m * v == r));
		}

		TEST_METHOD(mult11)
		{
			vec3f v = vec3f(5, -1, 2);
			mat3f m = mat3f(
				1, 7, 2,
				7, 0, -3,
				2, -3, -1
			);
			vec3f r = vec3f(2, 29, 11);
			Assert::IsTrue(all(m * v == r));
		}

		TEST_METHOD(mult12)
		{
			vec3f v = vec3f(5, -1, 2);
			mat3f m = mat3f(
				0, -4, 3,
				4, 0, -1,
				-3, 1, 0
			);
			vec3f r = vec3f(10, 18, -16);
			Assert::IsTrue(all(m * v == r));
		}

		TEST_METHOD(mult13)
		{
			mat4f a = mat4f(
				1, 2, 3, 4,
				5, 6, 7, 8,
				9, 10, 11, 12,
				13, 14, 15, 16
			);
			mat4f b = mat4f(
				5, 12, -3, 6,
				8, 3, 7, -2,
				-1, 5, 11, 9,
				0, 1, 2, 3
			);
			mat4f c = mat4f(
				18, 37, 52, 41,
				66, 121, 120, 105,
				114, 205, 188, 169,
				162, 289, 256, 233
			);
			Assert::IsTrue(a * b == c);
		}

		TEST_METHOD(mult14)
		{
			mat4f translate = mat4f(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, -4, 1
			);
			mat4f rotate = mat4f(
				5, 12, -3, 0,
				8, 3, 7, 0,
				-1, 5, 11, 0,
				0, 0, 0, 1
			);
			mat4f c = mat4f(
				5, 12, -3, 0,
				8, 3, 7, 0,
				-1, 5, 11, 0,
				0, 0, -4, 1
			);
			Assert::IsTrue(rotate * translate == c);
		}
	};
}