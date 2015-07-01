#pragma once

#include "fwddecl.h"
#include "Thread.h"
#include "MathUtil.h"
#include "Renderer.h"

namespace TX
{
	namespace RayTracer
	{
		class RenderTask{
		public:
			RenderTask(Renderer *renderer) : renderer(renderer){}
			inline void Render(int workerId){
				//renderer->Render(workerId);
			}
			static void Run(RenderTask *task, int workerId){
				task->Render(workerId);
			}
		public:
			Renderer *const renderer;
		};

		struct RenderTile {
			static const int SIZE = 32;
			const int xmin, ymin, xmax, ymax;
			RenderTile(int xmin, int ymin, int xmax, int ymax) 
				: xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax){}
		};

		class Synchronizer {
		public:
			void Init(int x, int y);
			inline void Abort(){ running = false; }
			inline void Resume(){ running = true; }
			inline bool Running(){ return running; }

			void ResetTiles(){ currentTile = 0; finished = false; }
			bool NextTile(RenderTile*& tile);
			void PreRenderSync(int workerId);
			void PostRenderSync(int workerId);

		private:
			std::vector<RenderTile> tiles;
			int currentTile;
			Lock syncLock;
			std::atomic_int preRenderCount, postRenderCount;	// number of threads that reached the barrier
			std::vector<HANDLE> preRenderEvents, postRenderEvents;
			bool finished;
			bool running;
		};
	}
}