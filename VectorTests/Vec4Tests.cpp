#include "pch.h"
#include "CppUnitTest.h"
#include "../Vector/Vec4.h"
#include "Spec.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace VectorTests
{
	TEST_CLASS(Vec4Tests)
	{
	public:
		TEST_METHOD(equals)
		{
			// Vectors a and b should be equal even if their difference is below FLT_EPSILON
			Vec4 a(1 - FLT_EPSILON + FLT_EPSILON / 2,
				1 - FLT_EPSILON + FLT_EPSILON / 2,
				1 - FLT_EPSILON + FLT_EPSILON / 2,
				1 - FLT_EPSILON + FLT_EPSILON / 2);
			Vec4 b(1, 1, 1, 1);

			// Explicitly using == operator to test it
			Assert::IsTrue(a == b);
		}
		TEST_METHOD(notEquals)
		{
			// Vectors a and b should not be equal if their difference is above or equal FLT_EPSILON
			Vec4 a(1 - FLT_EPSILON, 1 - FLT_EPSILON, 1 - FLT_EPSILON, 1 - FLT_EPSILON);
			Vec4 b(1, 1, 1, 1);

			// Explicitly using != operator to test it
			Assert::IsTrue(a != b);
		}
		TEST_METHOD(addition)
		{
			Vec4 a(1, 2, 3, 4);
			Vec4 b(3, 4, 5, 6);
			Vec4 result = a + b;
			Vec4 expected(4, 6, 8, 10);

			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(subtraction)
		{
			Vec4 a(2, 2, 2, 2);
			Vec4 b(1, 1, 1, 1);
			Vec4 result = a - b;
			Vec4 expected(1, 1, 1, 1);

			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(assignment) {
			Vec4 a(1, 2, 3, 4);
			Vec4 b(3, 4, 5, 6);
			a = b;
			Assert::AreEqual(a, b);
		}
		TEST_METHOD(assignmentbyVectorSum) {
			Vec4 a(1, 1, 1, 1);
			Vec4 b(2, 2, 2, 2);
			a += b;
			Vec4 expected(3, 3, 3, 3);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(assignmentbyVectorDifference) {
			Vec4 a(2, 2, 2, 2);
			Vec4 b(1, 1, 1, 1);
			a -= b;
			Vec4 expected(1, 1, 1, 1);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(assignmentbyScalarSum) {
			Vec4 a(1, 1, 1, 1);
			a += 2;
			Vec4 expected(3, 3, 3, 3);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(multiplicationByScalar)
		{
			Vec4 a(1, 1, 1, 1);
			Vec4 result = 2 * a;
			result = result * 2;
			Vec4 expected(4, 4, 4, 4);
			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(divisionByScalar)
		{
			Vec4 a(2, 2, 2, 2);
			Vec4 result = a / 2;
			Vec4 expected(1, 1, 1, 1);
			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(assignmentbyScalarDifference) {
			Vec4 a(3, 3, 3, 3);
			a -= 2;
			Vec4 expected(1, 1, 1, 1);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(assignmentbyScalarProduct) {
			Vec4 a(1, 1, 1, 1);
			a *= 4;
			Vec4 expected(4, 4, 4, 4);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(assignmentbyScalarQuotient) {
			Vec4 a(4, 4, 4, 4);
			a /= 2;
			Vec4 expected(2, 2, 2, 2);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(magnitude) {
			Vec4 a(4, 4, 4, 4);
			Assert::AreEqual(8.0f, a.magnitude());
		}
		TEST_METHOD(sqrMagnitude) {
			Vec4 a(4, 4, 4, 4);
			Assert::AreEqual(64.0f, a.sqrMagnitude());
		}
		TEST_METHOD(normalize) {
			Vec4 a(3, 3, 3, 3);
			Vec4 aNormalized = a.normalize();
			Assert::AreEqual(1.0f, aNormalized.magnitude());
		}
		TEST_METHOD(normalizeSpecialCase) {
			Vec4 a(98, 1109, 718, 865);
			Vec4 aNormalized = a.normalize();
			Assert::AreEqual(1.0f, aNormalized.magnitude());
		}
		TEST_METHOD(dotProduct) {
			// Perpendicular vectors should have a dot product of 0
			Vec4 a(1, 0, 0, 0);
			Vec4 b(0, 1, 0, 0);
			Assert::AreEqual(0.0f, dot(a, b));
		}
	};
}
