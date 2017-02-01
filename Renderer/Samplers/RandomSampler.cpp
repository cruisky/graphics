#include "stdafx.h"
#include "RandomSampler.h"
#include "TXBase/Math/Sample.h"

namespace TX{
	void RandomSampler::GetSamples(CameraSample *sample){
		sample->x = rng.Float();
		sample->y = rng.Float();
		for (int i = 0; i < sample->bufsize; i++){
			sample->buffer[i].u = rng.Float();
			sample->buffer[i].v = rng.Float();
			sample->buffer[i].w = rng.Float();
		}
	}
}
