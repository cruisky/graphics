#pragma once
#include "RenderingFwd.h"
#include <memory>
#include "Light.h"
#include "PrimitiveManager.h"

namespace TX{
	namespace Rendering
	{
		class Scene{
		public:
			Scene(std::shared_ptr<Camera> camera);
			
			void AddPrimitive(std::shared_ptr<Primitive> prim);
			void AddLight(std::shared_ptr<Light> light);

			// Initializes the scene after all lights & primitives are added
			void Construct();

			bool Intersect(const Ray& ray, Intersection& intxn) const;
			void PostIntersect(const Ray& ray, LocalGeo& geo) const;
			bool Occlude(const Ray& ray) const;

		public:
			std::vector<std::shared_ptr<Light>> lights;
			std::shared_ptr<Camera> camera;
		private:
			std::unique_ptr<PrimitiveManager> primmgr_;
			std::vector<std::shared_ptr<Primitive>> prims_;
		};
	}

}