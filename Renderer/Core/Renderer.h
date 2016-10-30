#pragma once
#include <memory>
#include "Synchronizer.h"
#include "Math/Sample.h"
#include "RendererConfig.h"

namespace TX {
	class Renderer {
	public:
		Renderer(const RendererConfig& config,
			const Scene& scene,
			Camera& camera,
			Film& film,
			IProgressMonitor *monitor=nullptr);
		~Renderer();
		bool Running();
		void Abort();
		void NewTask();
		void Render(int workerId, RNG& random);
		void RenderTiles(CameraSample& sample_buf, RNG& random);

		Renderer& Resize(int width, int height);
	public:
		const Scene& scene;
		Camera& camera;
		Film& film;
		const RendererConfig& config;
	private:
		RendererConfig runtimeConfig;
		std::unique_ptr<Tracer> tracer_;
		std::unique_ptr<Sampler> sampler_;
		std::unique_ptr<CameraSample> sample_buf_;
		Synchronizer thread_sync_;
		std::vector<std::shared_ptr<RenderTask>> tasks_;
		IProgressMonitor *monitor_;
	};
}
