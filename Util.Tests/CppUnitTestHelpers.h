#pragma once
#pragma warning(disable: 4996)

#include "CppUnitTest.h"
#include <cstdarg>

#include "System/Tools.h"
#include "Graphics/Color.h"
#include "Math/Ray.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"

#define repeat(i,n) for(int (i)=0;(i)<(n);++(i))
#define repeat_r(i,a,b) for(int (i)=(a);(i)<(b);++(i))
#define repeat_s(i,n,s) for(int (i)=0;(i)<(n);(i)+=(s))
#define repeat_rs(i,a,b,s) for(int (i)=(a);(i)<(b);(i)+=(s))

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TX
{
	namespace Tests
	{
		namespace Msg{
			inline std::wstring WStr(const std::string& str){
				return std::wstring(str.begin(), str.end());
			}
			
			inline void LogFormat(const char *fmt, ...){
				static char buf[512];
				va_list ap;
				va_start(ap, fmt);
				std::vsprintf(buf, fmt, ap);
				va_end(ap);
				Logger::WriteMessage(buf);
			}

			template <typename T>
			inline void Log(const T& t)
			{
				std::string s(TX::Str(t));
				Logger::WriteMessage(s.c_str());
			}

			template <typename T, typename... Args>
			inline void Log(const T& t, const Args&... args)
			{
				std::string s(TX::Str(t));
				Logger::WriteMessage(s.c_str());
				Log(args...);
			}

			template <typename T>
			inline const wchar_t* EQ(const T& expected, const T& actual, std::string msg = ""){
				return WStr(Str("Expected<", expected, ">, Actual<", actual, "> ", msg)).c_str();
			}
		}

		namespace Assertions {
			const float TOLERANCE_FLT = 1e-3f;
			const double TOLERANCE_DBL = 1e-6;

			inline void AreClose(float expected, float actual, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL){
				Assert::AreEqual(expected, actual, TOLERANCE_FLT, message, pLineInfo);
			}
			inline void AreClose(double expected, double actual, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL){
				Assert::AreEqual(expected, actual, TOLERANCE_DBL, message, pLineInfo);
			}
			inline void AreClose(const Vector4& expected, const Vector4& actual, const wchar_t *msg = nullptr){
				repeat(i, 4){
					Assert::AreEqual(expected[i], actual[i], TOLERANCE_FLT, msg ? msg : Msg::EQ(expected[i], actual[i]));
				}
			}
			inline void AreClose(const Color& expected, const Color& actual, const wchar_t *msg = nullptr){
				Assert::AreEqual(expected.r, actual.r, TOLERANCE_FLT, msg ? msg : Msg::EQ(expected, actual));
				Assert::AreEqual(expected.g, actual.g, TOLERANCE_FLT, msg ? msg : Msg::EQ(expected, actual));
				Assert::AreEqual(expected.b, actual.b, TOLERANCE_FLT, msg ? msg : Msg::EQ(expected, actual));
			}
			inline void AreClose(const Vector3& expected, const Vector3& actual, const wchar_t *msg = nullptr){
				Assert::AreEqual(expected.x, actual.x, TOLERANCE_FLT, msg ? msg : Msg::EQ(expected, actual));
				Assert::AreEqual(expected.y, actual.y, TOLERANCE_FLT, msg ? msg : Msg::EQ(expected, actual));
				Assert::AreEqual(expected.z, actual.z, TOLERANCE_FLT, msg ? msg : Msg::EQ(expected, actual));
			}
			inline void AreClose(const Matrix4x4& expected, const Matrix4x4& actual, const wchar_t *msg = nullptr){
				repeat(i, 4){
					Assertions::AreClose(expected[i], actual[i], msg ? msg : Msg::EQ(expected, actual));
				}
			}
		}
	}
}