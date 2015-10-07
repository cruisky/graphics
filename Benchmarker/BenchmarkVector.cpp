#include "stdafx.h"

//#define ENABLE_BENCHMARK
#include "Benchmarker.h"

namespace TX
{
	namespace Tests
	{
		BM_CLASS(Vector4_Mult_SSE) {
			Vector4 a, b;
			BM_SETUP(){
				a = RandomVector4(), b = RandomVector4();
			}
			BM_BODY(){
				__m128 x = _mm_loadu_ps((float *)a), y = _mm_loadu_ps((float *)b);
				x = _mm_mul_ps(x, y);
				_mm_storeu_ps((float *)a, x);
			}
		};

		BM_CLASS(Vector4_Mult){
			Vector4 a, b;
			BM_SETUP(){
				a = RandomVector4(), b = RandomVector4();
			}
			BM_BODY(){
				a *= b;
			}
		};

		BM_CLASS(Scalar4_Mult){
			float a[4], b[4];
			BM_SETUP(){
				memcpy(a, RandomVector4().f, 4 * sizeof(float));
				memcpy(b, RandomVector4().f, 4 * sizeof(float));
			}
			BM_BODY(){
				a[0] *= b[0]; a[1] *= b[1];	a[2] *= b[2]; a[3] *= b[3];
			}
		};

		BENCHMARK(Vector4_Mult,		PRECISION_HIGH, 2e8);
		BENCHMARK(Scalar4_Mult,		PRECISION_HIGH, 2e8);
		BENCHMARK(Vector4_Mult_SSE, PRECISION_HIGH, 2e8);
	}
}
