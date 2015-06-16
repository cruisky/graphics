// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"

#include <iostream>
#include <string>
#include <cstdarg>
#include <cstdlib>
#include "TestHelpers.h"
#include "Tools.h"
#include "RNG.h"

// Area to test
#include "Color.h"
#include "MathUtil.h"
#include "Matrix.h"
#include "Ray.h"
#include "Vector.h"

#define repeat(i,n) for(int (i)=0;(i)<(n);++(i))
#define repeat_range(i,a,b) for(int (i)=(a);(i)<(b);++(i))

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace TX;
