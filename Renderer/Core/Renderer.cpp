#include "stdafx.h"
#include "Renderer.h"
#include "RendererConfig.h"
#include "Image/Film.h"
#include "Scene/Camera.h"
#include "Math/Sample.h"
#include "Core/Scene.h"

namespace TX {
	Renderer::Renderer(
		const RendererConfig& config,
		const Scene& scene,
		Camera& camera,
		Film& film,
		IProgressMonitor *monitor)
		: config(config), scene(scene), camera(camera), film(film), monitor_(monitor) {
		ThreadScheduler::Instance()->StartAll();
		// Sample buffer
		sample_buf_ = std::make_unique<CameraSample>(10);	// should be enough to trace a ray
		// Init tiled rendering synchronizer
		thread_sync_.Init(config.width, config.height);
	}
	Renderer::~Renderer(){
		Abort();
		ThreadScheduler::Instance()->StopAll();
	}



	Renderer& Renderer::Resize(int width, int height) {
		if (runtimeConfig.width != width || runtimeConfig.height != height){
			bool wasRunning = this->Running();
			Abort();
			camera.Resize(width, height);
			film.Resize(width, height);
			thread_sync_.Init(width, height);
			if (wasRunning) {
				NewTask();
			}
		}
		return *this;
	}

	bool Renderer::Running() {
		return thread_sync_.Running() && ThreadScheduler::Instance()->taskCount > 0;
	}

	void Renderer::Abort(){
		thread_sync_.Abort();
		ThreadScheduler::Instance()->JoinAll();
	}

	void Renderer::NewTask(){
		if (monitor_) monitor_->Reset(float(config.samples_per_pixel * thread_sync_.TileCount()));
		film.Clear();
		thread_sync_.Resume();
		runtimeConfig = config;

		Resize(config.width, config.height);
		tracer_.reset(config.NewMethod());
		sampler_.reset(config.NewSampler());

		// generate sample offset for the current tracer
		tracer_->BakeSamples(&scene, sample_buf_.get());

		for (auto i = 0; i < ThreadScheduler::Instance()->ThreadCount(); i++){
			tasks_.push_back(std::make_shared<RenderTask>(this));
			ThreadScheduler::Instance()->AddTask(Task((Task::Func)&RenderTask::Run, tasks_[i].get()));
		}
	}

	void Renderer::Render(int workerId, RNG& random) {
		// duplicate sample buffer in each thread
		CameraSample sample_buf_dup(*sample_buf_);
		for (int i = 0; thread_sync_.Running() && i < runtimeConfig.samples_per_pixel; i++){
			// sync threads before and after each sample frame
			thread_sync_.PreRenderSync(workerId);
			RenderTiles(sample_buf_dup, random);
			thread_sync_.PostRenderSync(workerId);

			if (workerId == 0){
				film.ScalePixels();
				thread_sync_.ResetTiles();
			}
		}
		if (workerId == 0){
			film.ScalePixels();
			if (monitor_) monitor_->Finish();
		}
	}

	void Renderer::RenderTiles(CameraSample& sample_buf, RNG& random){
		RenderTile* tile;
		Ray ray;
		Color c;
		while (thread_sync_.NextTile(tile)){
			for (int y = tile->ymin; y < tile->ymax; y++){
				for (int x = tile->xmin; x < tile->xmax; x++){
					if (!thread_sync_.Running()) return;
					sampler_->GetSamples(&sample_buf);
					sample_buf.pix_x = x;
					sample_buf.pix_y = y;
					sample_buf.x += x;
					sample_buf.y += y;
					camera.GenerateRay(&ray, sample_buf.x, sample_buf.y);
					tracer_->Trace(&scene, ray, sample_buf, random, &c);
					film.Commit(sample_buf.x, sample_buf.y, c);
				}
			}
			if (monitor_) monitor_->UpdateInc();
		}
	}
}
