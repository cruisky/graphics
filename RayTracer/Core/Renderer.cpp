#include "stdafx.h"
#include "Renderer.h"
#include "Camera.h"
#include "Film.h"
#include "Sample.h"
#include "Color.h"
#include "Ray.h"

namespace Cruisky {
	namespace RayTracer {
		Renderer::Renderer() : 
			tracer_(new Tracer), sampler_(new RandomSampler){}

		void Renderer::Render(const Scene *scene, const Camera *camera, Film *film) {
			CameraSample cam_sample(10);
			Ray ray; 
			Color c;
			assert(film->Width() == camera->Width());
			assert(film->Height() == camera->Height());
			tracer_->SetScene(scene);
			for (int y = 0; y < film->Height(); y++){
				for (int x = 0; x < film->Width(); x++){
					sampler_->GetSamples(&cam_sample);
					cam_sample.pix_x = x;
					cam_sample.pix_y = y;
					cam_sample.x += x;
					cam_sample.y += y;
					camera->GenerateRay(&ray, cam_sample);
					tracer_->Trace(ray, cam_sample, &c);
					film->Commit(cam_sample, c);
				}
			}
		}
	}
}