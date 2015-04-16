#include "stdafx.h"
#include "Renderer.h"
#include "Camera.h"
#include "Film.h"
#include "Color.h"
#include "Ray.h"

namespace Cruisky {
	namespace RayTracer {
		Renderer::Renderer(Film *film) : film_(film){
			tracer_ = std::make_unique<Tracer>();
		}

		void Renderer::Render(const Scene *scene, const Camera *camera) {
			Sample sample;
			Ray ray; 
			Color c;
			sampler_.reset(new SimpleSampler(0, camera->Width(), 0, camera->Height()));
			film_->Resize(camera->Width(), camera->Height());
			tracer_->SetScene(scene);
			while (sampler_->GetSample(&sample, nullptr)){
				ray.Reset();
				camera->GenerateRay(&ray, sample);
				//std::cout << sample << "  ->  " << ray << std::endl;
				tracer_->Trace(ray, &c);
				film_->Commit(sample, c);
			}
		}
	}
}