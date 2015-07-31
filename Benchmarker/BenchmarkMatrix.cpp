#include "stdafx.h"

//#define ENABLE_BENCHMARK
#include "Benchmarker.h"

namespace TX
{
	namespace Tests{
		BM_CLASS(Matrix4x4_Mult) {
			Matrix4x4 a, b;
			BM_SETUP(){
				a = RandomMatrix(), b = RandomMatrix();
			}
			BM_BODY(){
				a *= b;
			}
		};
		BM_CLASS(Matrix4x4_Mult_SSE) {
		public:
			Matrix4x4 a, b;
			BM_SETUP(){
				a = RandomMatrix(), b = RandomMatrix();
			}
			BM_BODY(){
				__m128 a_line, b_line, r_line;
				for (int i = 0, j; i < 4; i++) {
					a_line = _mm_loadu_ps(a[0]);
					b_line = _mm_set1_ps(b[i][0]);
					r_line = _mm_mul_ps(a_line, b_line);
					for (j = 1; j < 4; j++) {
						a_line = _mm_loadu_ps(a[j]);
						b_line = _mm_set1_ps(b[i][j]);
						r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
					}
					_mm_storeu_ps(a[i], r_line);
				}
			}
		};
		BM_CLASS(Matrix4x4_Mult_Std) {
			Matrix4x4 a, b;
			BM_SETUP(){
				a = RandomMatrix(), b = RandomMatrix();
			}
			BM_BODY(){
				Vector4 temp;
				for (int i = 0; i < 4; i++){
					for (int j = 0; j < 4; j++)
						temp[j] =
						a[i][0] * b[0][j] +
						a[i][1] * b[1][j] +
						a[i][2] * b[2][j] +
						a[i][3] * b[3][j];
					a[i] = temp;
				}
			}
		};

		BENCHMARK(Matrix4x4_Mult,	  PRECISION_HIGH, 2e7);
		BENCHMARK(Matrix4x4_Mult_Std, PRECISION_HIGH, 2e7);
		BENCHMARK(Matrix4x4_Mult_SSE, PRECISION_HIGH, 2e7);
	}
}
