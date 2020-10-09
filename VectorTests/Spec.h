#pragma once

#include "CppUnitTest.h"
#include "../Vector/Vec2.h"
#include "../Vector/Vec3.h"
#include "../Vector/Vec4.h"

namespace Microsoft
{
    namespace VisualStudio
    {
        namespace CppUnitTestFramework
        {
            template<> static std::wstring ToString<Vec2>(const class Vec2& t) {
                return L"Vector2";
            }
            template<> static std::wstring ToString<Vec3>(const class Vec3& t) {
                return L"Vector3";
            }
            template<> static std::wstring ToString<Vec4>(const class Vec4& t) {
                return L"Vector3";
            }
        }
    }
}