#include "stdafx.h"
#include "Renderer.h"
#include "Camera.h"
#include "Film.h"
#include "Color.h"
#include "Ray.h"

namespace Cruisky {
	namespace RayTracer {
		Renderer::Renderer() : 
			tracer_(new Tracer), sampler_(new SimpleSampler){}

		void Renderer::Render(const Scene *scene, const Camera *camera, Film *film) {
			Sample sample;
			Ray ray; 
			Color c;
			sampler_.reset(new SimpleSampler(0, camera->Width(), 0, camera->Height()));
			assert(film->Width() == camera->Width());
			assert(film->Height() == camera->Height());
			film->Reset();
			tracer_->SetScene(scene);
			while (sampler_->GetSample(&sample, nullptr)){
				camera->GenerateRay(&ray, sample);
				tracer_->Trace(ray, &c);
				film->Commit(sample, c);
			}
		}
	}
}