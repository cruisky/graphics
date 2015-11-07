#include "stdafx.h"

//#define ENABLE_BENCHMARK
#include "Benchmarker.h"

namespace TX
{
	namespace Tests
	{
		BM_CLASS(Vec4_Mult_SSE) {
			Vec4 a, b;
			BM_SETUP(){
				a = RandomVec4(), b = RandomVec4();
			}
			BM_BODY(){
				__m128 x = _mm_loadu_ps((float *)a), y = _mm_loadu_ps((float *)b);
				x = _mm_mul_ps(x, y);
				_mm_storeu_ps((float *)a, x);
			}
		};

		BM_CLASS(Vec4_Mult){
			Vec4 a, b;
			BM_SETUP(){
				a = RandomVec4(), b = RandomVec4();
			}
			BM_BODY(){
				a *= b;
			}
		};

		BM_CLASS(Scalar4_Mult){
			float a[4], b[4];
			BM_SETUP(){
				std::memcpy(a, RandomVec4().data, 4 * sizeof(float));
				std::memcpy(b, RandomVec4().data, 4 * sizeof(float));
			}
			BM_BODY(){
				a[0] *= b[0]; a[1] *= b[1];	a[2] *= b[2]; a[3] *= b[3];
			}
		};

		BENCHMARK(Vec4_Mult,		PRECISION_HIGH, 2e8);
		BENCHMARK(Scalar4_Mult,		PRECISION_HIGH, 2e8);
		BENCHMARK(Vec4_Mult_SSE, PRECISION_HIGH, 2e8);
	}
}
