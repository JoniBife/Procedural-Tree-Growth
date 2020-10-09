#include "pch.h"
#include "CppUnitTest.h"
#include "../Vector/Vec3.h"
#include "Spec.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace VectorTests
{
	TEST_CLASS(Vec3Tests)
	{
	public:
		TEST_METHOD(equals)
		{
			// Vectors a and b should be equal even if their difference is below FLT_EPSILON
			Vec3 a(1 - FLT_EPSILON + FLT_EPSILON / 2, 1 - FLT_EPSILON + FLT_EPSILON / 2, 1 - FLT_EPSILON + FLT_EPSILON / 2);
			Vec3 b(1, 1, 1);

			// Explicitly using == operator to test it
			Assert::IsTrue(a == b);
		}
		TEST_METHOD(notEquals)
		{
			// Vectors a and b should not be equal if their difference is above or equal FLT_EPSILON
			Vec3 a(1 - FLT_EPSILON, 1 - FLT_EPSILON, 1 - FLT_EPSILON);
			Vec3 b(1, 1, 1);

			// Explicitly using != operator to test it
			Assert::IsTrue(a != b);
		}
		TEST_METHOD(addition)
		{
			Vec3 a(1, 2, 3);
			Vec3 b(3, 4, 5);
			Vec3 result = a + b;
			Vec3 expected(4, 6, 8);

			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(subtraction)
		{
			Vec3 a(2, 2, 2);
			Vec3 b(1, 1, 1);
			Vec3 result = a - b;
			Vec3 expected(1, 1, 1);

			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(assignment) {
			Vec3 a(1, 2, 3);
			Vec3 b(3, 4, 5);
			a = b;
			Assert::AreEqual(a, b);
		}
		TEST_METHOD(assignmentbyVectorSum) {
			Vec3 a(1, 1, 1);
			Vec3 b(2, 2, 2);
			a += b;
			Vec3 expected(3, 3, 3);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(assignmentbyVectorDifference) {
			Vec3 a(2, 2, 2);
			Vec3 b(1, 1, 1);
			a -= b;
			Vec3 expected(1, 1, 1);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(assignmentbyScalarSum) {
			Vec3 a(1, 1, 1);
			a += 2;
			Vec3 expected(3, 3, 3);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(multiplicationByScalar)
		{
			Vec3 a(1, 1, 1);
			Vec3 result = 2 * a;
			result = result * 2;
			Vec3 expected(4, 4, 4);
			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(divisionByScalar)
		{
			Vec3 a(2, 2, 2);
			Vec3 result = a / 2;
			Vec3 expected(1, 1, 1);
			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(assignmentbyScalarDifference) {
			Vec3 a(3, 3, 3);
			a -= 2;
			Vec3 expected(1, 1, 1);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(assignmentbyScalarProduct) {
			Vec3 a(1, 1, 1);
			a *= 4;
			Vec3 expected(4, 4, 4);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(assignmentbyScalarQuotient) {
			Vec3 a(4, 4, 4);
			a /= 2;
			Vec3 expected(2, 2, 2);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(magnitude) {
			Vec3 a(4, 4, 2);
			Assert::AreEqual(6.0f, a.magnitude());
		}
		TEST_METHOD(sqrMagnitude) {
			Vec3 a(4, 4, 2);
			Assert::AreEqual(36.0f, a.sqrMagnitude());
		}
		TEST_METHOD(normalize) {
			Vec3 a(3, 3, 3);
			Vec3 aNormalized = a.normalize();
			Assert::AreEqual(1.0f, aNormalized.magnitude());
		}
		TEST_METHOD(normalizeSpecialCase) {
			Vec3 a(98, 1109, 718);
			Vec3 aNormalized = a.normalize();
			Assert::AreEqual(1.0f, aNormalized.magnitude());
		}

		TEST_METHOD(dotProduct) {
			// Perpendicular vectors should have a dot product of 0
			Vec3 a(1, 0, 0);
			Vec3 b(0, 1, 0);
			Assert::AreEqual(0.0f, dot(a, b));
		}

		TEST_METHOD(crossProduct) {
			Vec3 a(9, 10, 5);
			Vec3 b(6, 2, 20);
			Vec3 result = cross(a, b);
			// The resulting vector of the cross product should be a vector perpendicular to both of them
			Assert::IsTrue(dot(a,result) == 0.0f && dot(b,result) == 0.0f);
		}


		// a X (b X c) = (a . c)b - (a . b)c

		// 10 Random Vectors * 3

		// 1 or more Zero Vectors

		TEST_METHOD(challengeNoZeros) {
			for (int i = 0; i < 10; ++i) {
				Vec3 a = Vec3::random();
				Vec3 b = Vec3::random();
				Vec3 c = Vec3::random();

				Vec3 left = cross(a, cross(b, c));
				Vec3 right = dot(a,c)*b - dot(a,b)*c;

				Assert::AreEqual(left, right);
			}
		}

		TEST_METHOD(challengeZeros) {
			for (int i = 0; i < 10; ++i) {
				Vec3 a = Vec3::random();
				Vec3 b = Vec3::random();
				Vec3 c = Vec3::ZERO;

				Vec3 left = cross(a, cross(b, c));
				Vec3 right = dot(a, c) * b - dot(a, b) * c;

				Assert::AreEqual(left, right);
			}
		}
	};
}
