// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "txbase_tests/targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"

#include <iostream>
#include <string>
#include <cstdarg>
#include <cstdlib>
#include "cpp_unittest_helper.h"
#include "txbase/misc/randomdata.h"
#include "txbase/sys/tools.h"
#include "txbase/math/random.h"

// Area to test
#include "txbase/math/base.h"
#include "txbase/math/matrix.h"
#include "txbase/math/vector.h"
#include "txbase/math/quaternion.h"
#include "txbase/math/ray.h"
#include "txbase/math/color.h"
#include "txbase/sse/sse.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace TX;
