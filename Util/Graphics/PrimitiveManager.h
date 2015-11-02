#pragma once
#include "Util.h"
#include <memory>
namespace TX
{
	class PrimitiveManager {
	public:
		PrimitiveManager() {}
		virtual ~PrimitiveManager(){}

		inline void Construct(const std::vector<std::shared_ptr<Primitive>>& prims) {
			prims_ = &prims;
			Build();
		}
		virtual bool Intersect(const Ray& ray, Intersection& intxn) const = 0;
		virtual bool Occlude(const Ray& ray) const = 0;
	protected:
		virtual void Build() = 0;
	protected:
		const std::vector<std::shared_ptr<Primitive>> *prims_;
	};
}
