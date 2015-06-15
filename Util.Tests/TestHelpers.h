#pragma once

#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TX
{
	inline void LogFormat(const char *fmt, ...){
		static char buf[512];
		va_list ap;
		vsprintf_s(buf, fmt, ap);
		Logger::WriteMessage(buf);
	}

	template <typename T>
	inline void Log(T t)
	{
		string s(TX::ToString(t));
		Logger::WriteMessage(s.c_str());
	}

	template<typename T, typename... Args>
	inline void Log(T t, Args... args)
	{
		string s(TX::ToString(t));
		Logger::WriteMessage(s.c_str());
		Log(args...);
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

		inline void AreNotClose(float expected, float actual, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL){
			Assert::AreNotEqual(expected, actual, TOLERANCE_FLT, message, pLineInfo);
		}
		inline void AreNotClose(double expected, double actual, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL){
			Assert::AreNotEqual(expected, actual, TOLERANCE_DBL, message, pLineInfo);
		}
	}
}