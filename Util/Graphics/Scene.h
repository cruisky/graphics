#pragma once
#include "Util.h"
#include <memory>
#include "Light.h"
#include "PrimitiveManager.h"

namespace TX{
	class Scene{
	public:
		Scene();

		void AddPrimitive(std::shared_ptr<Primitive> prim);
		void AddLight(std::shared_ptr<Light> light);

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
		std::unique_ptr<PrimitiveManager> primmgr_;
		std::vector<std::shared_ptr<Primitive>> prims_;
	};
}
