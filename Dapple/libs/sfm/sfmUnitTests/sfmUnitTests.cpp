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
			vec4f a = vec4f(0);

			Assert::AreEqual(0.0f, a.x());
			Assert::AreEqual(0.0f, a.y());
			Assert::AreEqual(0.0f, a.z());
			Assert::AreEqual(0.0f, a.w());
		}

		TEST_METHOD(Constructor2)
		{
			vec4f a = vec4f(0, 1, 2, 3);

			Assert::AreEqual(0.0f, a.x());
			Assert::AreEqual(1.0f, a.y());
			Assert::AreEqual(2.0f, a.z());
			Assert::AreEqual(3.0f, a.w());
		}

		TEST_METHOD(Constructor3)
		{
			float p[4] = {0, 1, 2, 3};
			vec4f a = vec4f(p);

			Assert::AreEqual(0.0f, a.x());
			Assert::AreEqual(1.0f, a.y());
			Assert::AreEqual(2.0f, a.z());
			Assert::AreEqual(3.0f, a.w());
		}

		TEST_METHOD(Swivel1)
		{
			vec4f a = vec4f(0, 1, 2, 3);

			a = a.yzxw();

			Assert::AreEqual(1.0f, a.x());
			Assert::AreEqual(2.0f, a.y());
			Assert::AreEqual(0.0f, a.z());
			Assert::AreEqual(3.0f, a.w());
		}

		TEST_METHOD(Swivel2)
		{
			vec4f a = vec4f(0, 1, 2, 3);

			a = a.zxyw();

			Assert::AreEqual(2.0f, a.x());
			Assert::AreEqual(0.0f, a.y());
			Assert::AreEqual(1.0f, a.z());
			Assert::AreEqual(3.0f, a.w());
		}

		TEST_METHOD(Swivel3)
		{
			vec4f a = vec4f(0, 1, 2, 3);

			a = a.wzyx();

			Assert::AreEqual(3.0f, a.x());
			Assert::AreEqual(2.0f, a.y());
			Assert::AreEqual(1.0f, a.z());
			Assert::AreEqual(0.0f, a.w());
		}

		TEST_METHOD(Store1)
		{
			vec4f a = vec4f(0, 1, 2, 3);

			float p[4] = { 0, 0, 0, 0 };

			a.store(p);

			Assert::AreEqual(0.0f, p[0]);
			Assert::AreEqual(1.0f, p[1]);
			Assert::AreEqual(2.0f, p[2]);
			Assert::AreEqual(3.0f, p[3]);
		}

		TEST_METHOD(SetX)
		{
			vec4f a = vec4f(0);

			a.setX(1.0f);

			Assert::AreEqual(1.0f, a.x());
		}
		TEST_METHOD(SetY)
		{
			vec4f a = vec4f(0);

			a.setY(1.0f);

			Assert::AreEqual(1.0f, a.y());
		}
		TEST_METHOD(SetZ)
		{
			vec4f a = vec4f(0);

			a.setZ(1.0f);

			Assert::AreEqual(1.0f, a.z());
		}
		TEST_METHOD(SetW)
		{
			vec4f a = vec4f(0);

			a.setW(1.0f);

			Assert::AreEqual(1.0f, a.w());
		}

		TEST_METHOD(IndexOperator)
		{
			vec4f a = vec4f(0, 1, 2, 3);

			Assert::AreEqual(0.0f, a[0]);
			Assert::AreEqual(1.0f, a[1]);
			Assert::AreEqual(2.0f, a[2]);
			Assert::AreEqual(3.0f, a[3]);
		}

		TEST_METHOD(add1)
		{
			vec4f a = vec4f(1, 1, 1, 1);
			vec4f b = vec4f(2, 3, 4, 5);

			vec4f r = a + b;

			Assert::AreEqual(3.0f, r.x());
			Assert::AreEqual(4.0f, r.y());
			Assert::AreEqual(5.0f, r.z());
			Assert::AreEqual(6.0f, r.w());
		}

		TEST_METHOD(add2)
		{
			vec4f a = vec4f(5, 7, 2, 4);
			vec4f b = vec4f(2, 3, 4, 5);

			vec4f r = a + b;

			Assert::AreEqual(7.0f, r.x());
			Assert::AreEqual(10.0f, r.y());
			Assert::AreEqual(6.0f, r.z());
			Assert::AreEqual(9.0f, r.w());
		}

		TEST_METHOD(add3)
		{
			vec4f a = vec4f(0,1,2,3);
			vec4f b = vec4f(-1,-3,-5,-7);

			vec4f r = a + b;

			Assert::AreEqual(-1.0f, r.x());
			Assert::AreEqual(-2.0f, r.y());
			Assert::AreEqual(-3.0f, r.z());
			Assert::AreEqual(-4.0f, r.w());
		}

		TEST_METHOD(add4)
		{
			vec4f a = vec4f(-1,-2,-3,-4);
			vec4f b = vec4f(-2,-3,-4,-5);

			vec4f r = a + b;

			Assert::AreEqual(-3.0f, r.x());
			Assert::AreEqual(-5.0f, r.y());
			Assert::AreEqual(-7.0f, r.z());
			Assert::AreEqual(-9.0f, r.w());
		}

		TEST_METHOD(sub1)
		{
			vec4f a = vec4f(1, 1, 1, 1);
			vec4f b = vec4f(2, 3, 4, 5);

			vec4f r = a - b;

			Assert::AreEqual(-1.0f, r.x());
			Assert::AreEqual(-2.0f, r.y());
			Assert::AreEqual(-3.0f, r.z());
			Assert::AreEqual(-4.0f, r.w());
		}

		TEST_METHOD(sub2)
		{
			vec4f a = vec4f(5, 7, 2, 4);
			vec4f b = vec4f(2, 3, 4, 5);

			vec4f r = a - b;

			Assert::AreEqual(3.0f, r.x());
			Assert::AreEqual(4.0f, r.y());
			Assert::AreEqual(-2.0f, r.z());
			Assert::AreEqual(-1.0f, r.w());
		}

		TEST_METHOD(sub3)
		{
			vec4f a = vec4f(0, 1, 2, 3);
			vec4f b = vec4f(-1, -3, -5, -7);

			vec4f r = a - b;

			Assert::AreEqual(1.0f, r.x());
			Assert::AreEqual(4.0f, r.y());
			Assert::AreEqual(7.0f, r.z());
			Assert::AreEqual(10.0f, r.w());
		}

		TEST_METHOD(sub4)
		{
			vec4f a = vec4f(-1, -2, -3, -4);
			vec4f b = vec4f(-2, -3, -4, -5);

			vec4f r = a - b;

			Assert::AreEqual(1.0f, r.x());
			Assert::AreEqual(1.0f, r.y());
			Assert::AreEqual(1.0f, r.z());
			Assert::AreEqual(1.0f, r.w());
		}
	};
}
