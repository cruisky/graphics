#pragma once

#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TX
{
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
	

	namespace Msg{
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

		inline void AreNotClose(float expected, float actual, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL){
			Assert::AreNotEqual(expected, actual, TOLERANCE_FLT, message, pLineInfo);
		}
		inline void AreNotClose(double expected, double actual, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL){
			Assert::AreNotEqual(expected, actual, TOLERANCE_DBL, message, pLineInfo);
		}
	}
}