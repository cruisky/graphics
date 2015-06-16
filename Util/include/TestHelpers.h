#pragma once

#include "CppUnitTest.h"

#define repeat(i,n) for(int (i)=0;(i)<(n);++(i))
#define repeat_range(i,a,b) for(int (i)=(a);(i)<(b);++(i))

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TX
{
	class RNG;
	class Color;
	class Ray;
	class Vector3;
	class Matrix4x4;

	namespace Tests
	{
		extern RNG rng;

		float RandomFloat(bool allowzero = true, float absmax = 1.f);
		Color RandomColor(float absmax = 1.f);
		Vector3 RandomVector(bool allowzero = true, float absmax = 1e2f);
		Ray RandomRay(float lengthmax = 1e2f);
		Matrix4x4 RandomMatrix(float absmax = 1e2f);

		namespace Msg{
			inline std::wstring WStr(const std::string& str){
				return std::wstring(str.begin(), str.end());
			}
			
			inline void LogFormat(const char *fmt, ...){
				static char buf[512];
				va_list ap;
				vsprintf_s(buf, fmt, ap);
				Logger::WriteMessage(buf);
			}

			template <typename T>
			inline void Log(T t)
			{
				std::string s(TX::Str(t));
				Logger::WriteMessage(s.c_str());
			}

			template<typename T, typename... Args>
			inline void Log(T t, Args... args)
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
			const float TOLERANCE_FLT = 1e-4f;
			const double TOLERANCE_DBL = 1e-6;

			inline void AreClose(float expected, float actual, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL){
				Assert::AreEqual(expected, actual, TOLERANCE_FLT, message, pLineInfo);
			}
			inline void AreClose(double expected, double actual, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL){
				Assert::AreEqual(expected, actual, TOLERANCE_DBL, message, pLineInfo);
			}
			void AreClose(Color expected, Color actual, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL);
			void AreClose(Vector3 expected, Vector3 actual, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL);
			void AreClose(Matrix4x4 expected, Matrix4x4 actual, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL);
		}
	}
}