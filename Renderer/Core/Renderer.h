#pragma once
#include <memory>
#include "Synchronizer.h"
#include "Math/Sample.h"
#include "RendererConfig.h"

namespace TX {
	class Renderer {
	public:
		Renderer(const RendererConfig& config,
			std::shared_ptr<Scene> scene,
			std::shared_ptr<Camera> camera,
			std::shared_ptr<Film> film,
			std::shared_ptr<IProgressMonitor> monitor=nullptr);
		~Renderer();
		bool Running();
		void Abort();
		void NewTask();
		void Render(int workerId, RNG& random);
		void RenderTiles(CameraSample& sample_buf, RNG& random);

		inline const RendererConfig& Config(){ return config_; }
		Renderer& Config(const RendererConfig& config);
		Renderer& Resize(int width, int height);
	public:
		std::shared_ptr<Scene> scene;
		std::shared_ptr<Camera> camera;
		std::shared_ptr<Film> film;
	private:
		RendererConfig config_;
		std::unique_ptr<Tracer> tracer_;
		std::unique_ptr<Sampler> sampler_;
		std::unique_ptr<CameraSample> sample_buf_;
		Synchronizer thread_sync_;
		std::vector<std::shared_ptr<RenderTask>> tasks_;
		std::shared_ptr<IProgressMonitor> monitor_;
	};
}
