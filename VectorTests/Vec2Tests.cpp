#include "pch.h"
#include "CppUnitTest.h"
#include "../Vector/Vec2.h"
#include "Spec.h"
#include <iostream>
#include <glm/glm.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace VectorTests
{
	TEST_CLASS(Vec2Tests)
	{
	public:
		TEST_METHOD(equals) 
		{
			// Vectors a and b should be equal even if their difference is below FLT_EPSILON
			Vec2 a(1 - FLT_EPSILON + FLT_EPSILON/2, 1 - FLT_EPSILON + FLT_EPSILON/2);
			Vec2 b(1, 1);

			// Explicitly using == operator to test it
			Assert::IsTrue(a == b);
		}
		TEST_METHOD(notEquals)
		{
			// Vectors a and b should not be equal if their difference is above or equal FLT_EPSILON
			Vec2 a(1 - FLT_EPSILON, 1 - FLT_EPSILON);
			Vec2 b(1, 1);

			// Explicitly using != operator to test it
			Assert::IsTrue(a != b);
		}
		TEST_METHOD(addition)
		{
			Vec2 a(1, 2);
			Vec2 b(3, 4);
			Vec2 result = a + b;
			Vec2 expected(4, 6);

			Assert::AreEqual(expected, result);
		}	
		TEST_METHOD(subtraction)
		{
			Vec2 a(2, 2);
			Vec2 b(1, 1);
			Vec2 result = a - b;
			Vec2 expected(1, 1);

			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(assignment) {
			Vec2 a(1, 2);
			Vec2 b(3, 4);
			a = b;
			Assert::AreEqual(a, b);
		}
		TEST_METHOD(assignmentbyVectorSum) {
			Vec2 a(1, 1);
			Vec2 b(2, 2);
			a += b;
			Vec2 expected(3, 3);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(assignmentbyVectorDifference) {
			Vec2 a(2, 2);
			Vec2 b(1, 1);
			a -= b;
			Vec2 expected(1, 1);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(assignmentbyScalarSum) {
			Vec2 a(1, 1);
			a += 2;
			Vec2 expected(3, 3);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(multiplicationByScalar)
		{
			Vec2 a(1, 1);
			Vec2 result = 2 * a;
			result = result * 2;
			Vec2 expected(4, 4);
			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(divisionByScalar)
		{
			Vec2 a(2, 2);
			Vec2 result = a / 2;
			Vec2 expected(1, 1);
			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(assignmentbyScalarDifference) {
			Vec2 a(3, 3);
			a -= 2;
			Vec2 expected(1, 1);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(assignmentbyScalarProduct) {
			Vec2 a(1, 1);
			a *= 4;
			Vec2 expected(4, 4);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(assignmentbyScalarQuotient) {
			Vec2 a(4, 4);
			a /= 2;
			Vec2 expected(2, 2);
			Assert::AreEqual(expected, a);
		}
		TEST_METHOD(magnitude) {
			Vec2 a(3, 4);
			Assert::AreEqual(5.0f, a.magnitude());
		}
		TEST_METHOD(sqrMagnitude) {
			Vec2 a(3, 4);
			Assert::AreEqual(25.0f, a.sqrMagnitude());
		}
		TEST_METHOD(normalize) {
			Vec2 a(3, 3);
			Vec2 aNormalized = a.normalize();
			Assert::AreEqual(1.0f, aNormalized.magnitude());
		}
		TEST_METHOD(normalizeSpecialCase) {
			Vec2 a(98, 1109);
			Vec2 aNormalized = a.normalize();
			Assert::AreEqual(1.0f, aNormalized.magnitude());
		}
		TEST_METHOD(normalizeSpecialCaseGLM) {
			glm::dvec2  a(98, 1109);
			glm::dvec2  b = glm::normalize(a);
			Assert::AreEqual(1.0, glm::length(b));
		}
		TEST_METHOD(dotProduct) {
			// Perpendicular vectors should have a dot product of 0
			Vec2 a(1, 0);
			Vec2 b(0, 1);
			Assert::AreEqual(0.0f, dot(a, b));
		}
	};
}
