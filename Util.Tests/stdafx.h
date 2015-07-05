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
#include "Misc/TestHelpers.h"
#include "System/Tools.h"
#include "Math/RNG.h"

// Area to test
#include "Math/MathUtil.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Math/SIMD.h"
#include "Graphics/Ray.h"
#include "Graphics/Color.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace TX;
