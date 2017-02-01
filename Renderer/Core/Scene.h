#pragma once
#include "Util.h"
#include <memory>
#include "PrimitiveManager.h"

namespace TX {
	class Scene {
	public:
		Scene(std::unique_ptr<PrimitiveManager> primmgr);

		void AddPrimitive(std::shared_ptr<Primitive> prim);
		void AddLight(std::shared_ptr<Light> light);
		void GetPrimitives(std::vector<std::shared_ptr<Primitive>>& result) const;

		/// <summary>
		/// Initializes the scene after all lights & primitives are added
		/// </summary>
		void Construct();

		bool Intersect(const Ray& ray, Intersection& intxn) const;
		void PostIntersect(const Ray& ray, LocalGeo& geo) const;
		bool Occlude(const Ray& ray) const;
	public:
		std::vector<std::shared_ptr<Light>> lights;
	private:
		std::vector<std::shared_ptr<Primitive>> prims_;
		std::unique_ptr<PrimitiveManager> primmgr_;
	};
}
