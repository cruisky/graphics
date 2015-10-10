#pragma once
#include "System/Thread.h"
#include "Math/Base.h"
#include "Math/Random.h"

namespace TX
{
	class Renderer;
	class RenderTask{
	public:
		RenderTask(Renderer *renderer) : renderer(renderer){}

		static void Run(RenderTask *task, int workerId){
			task->Render(workerId);
		}
	protected:
		void Render(int workerId);
	public:
		Renderer *renderer;
	private:
		RNG random;
	};

	struct RenderTile {
		static const int SIZE = 64;
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
		int TileCount();
		void PreRenderSync(int workerId);
		void PostRenderSync(int workerId);

	private:
		std::vector<RenderTile> tiles;
		uint currentTile;
		Lock syncLock;
		std::atomic_int preRenderCount, postRenderCount;	// number of threads that reached the barrier
		std::vector<HANDLE> preRenderEvents, postRenderEvents;
		bool finished;
		bool running;
	};
}
