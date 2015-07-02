#include "stdafx.h"

#include "Synchronizer.h"
#include "Renderer.h"

namespace TX
{
	namespace RayTracer
	{
		void RenderTask::Render(int workerId) {
			renderer->Render(workerId, random);
		}
		void Synchronizer::Init(int x, int y){
			tiles.clear();
			currentTile = 0;
			for (int i = 0; i < y; i += RenderTile::SIZE){
				for (int j = 0; j < x; j += RenderTile::SIZE){
					int xmin = j;
					int ymin = i;
					int xmax = Math::Min(x, j + RenderTile::SIZE);
					int ymax = Math::Min(y, i + RenderTile::SIZE);
					tiles.push_back(RenderTile(xmin, ymin, xmax, ymax));
				}
			}
			for (auto& e : preRenderEvents) CloseHandle(e);
			for (auto& e : postRenderEvents) CloseHandle(e);
			preRenderEvents.resize(ThreadScheduler::Instance()->ThreadCount());
			postRenderEvents.resize(ThreadScheduler::Instance()->ThreadCount());
			for (auto& e : preRenderEvents) e = CreateEvent(NULL, TRUE, FALSE, NULL);
			for (auto& e : postRenderEvents) e = CreateEvent(NULL, TRUE, FALSE, NULL);
			running = true;
			finished = false;
		}
		bool Synchronizer::NextTile(RenderTile*& tile){
			LockGuard scope(syncLock);
			if (currentTile < tiles.size()){
				tile = &tiles[currentTile++];
#ifdef _DEBUG
				printf("rendering tile %d: %d,%d -> %d,%d\n", currentTile, tile->xmin, tile->ymin, tile->xmax, tile->ymax);
#endif
				return true;
			}
			return false;
		}
		int Synchronizer::TileCount(){
			return tiles.size();
		}
		void Synchronizer::PreRenderSync(int workerId){
			SetEvent(preRenderEvents[workerId]);
			while (WaitForMultipleObjects(preRenderEvents.size(), preRenderEvents.data(), true, 50) == WAIT_TIMEOUT)
				if (!running) return;
			if (++preRenderCount == preRenderEvents.size()){
				preRenderCount = 0;
				for (auto& e : preRenderEvents) ResetEvent(e);
			}
		}
		void Synchronizer::PostRenderSync(int workerId){
			SetEvent(postRenderEvents[workerId]);
			while (WaitForMultipleObjects(postRenderEvents.size(), postRenderEvents.data(), true, 50) == WAIT_TIMEOUT)
				if (!running) return;
			if (++postRenderCount == postRenderEvents.size()){
				postRenderCount = 0;
				for (auto& e : postRenderEvents) ResetEvent(e);
			}
		}
	}
}
