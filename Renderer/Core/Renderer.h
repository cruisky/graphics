#pragma once
#include <memory>
#include "Synchronizer.h"
#include "Math/Sample.h"
#include "RendererConfig.h"

namespace TX {
	class Renderer {
	public:
		Renderer(const RendererConfig& config, shared_ptr<Scene> scene, shared_ptr<Film> film, shared_ptr<IProgressMonitor> monitor=nullptr);
		void Abort();
		void NewTask();
		void Render(int workerId, RNG& random);
		void RenderTiles(CameraSample& sample_buf, RNG& random);

		inline const RendererConfig& Config(){ return config_; }
		Renderer& Config(const RendererConfig& config);
		Renderer& Resize(int width, int height);
	public:
		shared_ptr<Scene> scene;
		shared_ptr<Film> film;
	private:
		RendererConfig config_;
		unique_ptr<Tracer> tracer_;
		unique_ptr<Sampler> sampler_;
		unique_ptr<CameraSample> sample_buf_;
		Synchronizer thread_sync_;
		vector<shared_ptr<RenderTask>> tasks_;
		shared_ptr<IProgressMonitor> monitor_;
	};
}
