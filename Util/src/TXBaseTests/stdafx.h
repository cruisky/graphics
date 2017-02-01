// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "TXBaseTests/targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"

#include <iostream>
#include <string>
#include <cstdarg>
#include <cstdlib>
#include "CppUnitTestHelpers.h"
#include "TXBase/Misc/RandomData.h"
#include "TXBase/System/Tools.h"
#include "TXBase/Math/Random.h"

// Area to test
#include "TXBase/Math/Base.h"
#include "TXBase/Math/Matrix.h"
#include "TXBase/Math/Vector.h"
#include "TXBase/Math/Quaternion.h"
#include "TXBase/Math/Ray.h"
#include "TXBase/Math/Color.h"
#include "TXBase/SSE/SSE.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace TX;
