#pragma once
#pragma warning(disable: 4996)

#include "CppUnitTest.h"
#include <cstdarg>

#include "System/Tools.h"
#include "Math/Color.h"
#include "Math/Ray.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "SSE/SSE.h"

#define repeat(i,n) for(auto (i)=0;(i)<(n);++(i))
#define repeat_r(i,a,b) for(auto (i)=(a);(i)<(b);++(i))
#define repeat_s(i,n,s) for(auto (i)=0;(i)<(n);(i)+=(s))
#define repeat_rs(i,a,b,s) for(auto (i)=(a);(i)<(b);(i)+=(s))

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TX
{
	namespace Tests
	{
		namespace Msg {
			inline void LogFormat(const char *fmt, ...) {
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
			inline std::string EQ(const T& expected, const T& actual, const std::string& msg = std::string()) {
				return Str("\nExpected: <", expected, ">\nActual: <", actual, ">\n", msg);
			}

			template <typename T>
			inline const wchar_t* EQ_(const T& expected, const T& actual, const std::string& msg = std::string()) {
				return WStr(EQ(expected, actual, msg)).c_str();
			}
		}

		namespace Assertions {
			static const struct {
				constexpr inline operator float() const { return 1e-3f; }
				constexpr inline operator double() const { return 1e-6; }
			} TOLERANCE;

			inline void AreClose(float expected, float actual, const std::string& msg = std::string()) {
				Assert::AreEqual(expected, actual, float(TOLERANCE), Msg::EQ_(expected, actual, msg));
			}

			inline void AreClose(double expected, double actual, const std::string& msg = std::string()) {
				Assert::AreEqual(expected, actual, double(TOLERANCE), Msg::EQ_(expected, actual, msg));
			}

			template <typename T>
			inline void AreEqual(const T& expected, const T& actual, const std::string& msg = std::string()) {
				Assert::AreEqual(expected, actual, WStr(msg).c_str());
			}

			template <typename T>
			inline void AreClose(const T& expected, const T& actual, const std::string& msg = std::string()) {
				throw "not implemented";
			}

			template<typename T>
			inline void AreClose(const T& expected, const T& actual, const uint elemCount, const std::string& msg = std::string()) {
				repeat(i, elemCount)
					Assertions::AreClose(expected[i], actual[i], Msg::EQ(expected, actual, msg));
			}
			template<typename T>
			inline void AreEqual(const T& expected, const T& actual, const uint elemCount, const std::string& msg = std::string()) {
				repeat(i, elemCount)
					Assertions::AreEqual(expected[i], actual[i], Msg::EQ(expected, actual, msg));
			}

			template<> inline void AreClose<Quaternion>(const Quaternion& expected, const Quaternion& actual, const std::string& msg) {
#define signof(f) ((f) >= 0 ? 1 : -1)
				// might get a negative version, but they are still equivalent
				bool negate = false;
				repeat(i, 4)
					if (Math::Abs(expected.q[i] - actual.q[i]) > float(TOLERANCE) && signof(expected.w) != signof(actual.w))
						negate = true;
#undef signof
				repeat(i, 4)
					Assert::AreEqual(
						negate ? -expected.q[i] : expected.q[i],
						actual.q[i], TOLERANCE, Msg::EQ_(expected, actual, msg));
			}
			template<> inline void AreClose<Vec4>(const Vec4& expected, const Vec4& actual, const std::string& msg){
				Assertions::AreClose(expected, actual, 4, msg);
			}
			template<> inline void AreClose<Color>(const Color& expected, const Color& actual, const std::string& msg) {
				Assertions::AreClose(expected, actual, 3, msg);
			}
			template<> inline void AreClose<Vec3>(const Vec3& expected, const Vec3& actual, const std::string& msg){
				Assertions::AreClose(expected, actual, 3, msg);
			}
			template<> inline void AreClose<Matrix4x4>(const Matrix4x4& expected, const Matrix4x4& actual, const std::string& msg){
				Assertions::AreClose(expected, actual, 4, msg);
			}


			template<> inline void AreEqual<SSE::V4Int>(const SSE::V4Int& expected, const SSE::V4Int& actual, const std::string& msg) {
				Assertions::AreEqual(expected, actual, 4, msg);
			}
			template<> inline void AreClose<SSE::V4Float>(const SSE::V4Float& expected, const SSE::V4Float& actual, const std::string& msg) {
				Assertions::AreClose(expected, actual, 4, msg);
			}
		}
	}
}
