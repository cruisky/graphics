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
		virtual bool Intersect(const Ray& ray, Intersection& intxn) const;
		virtual bool Occlude(const Ray& ray) const;
	protected:
		virtual void Build() {}
	protected:
		const std::vector<std::shared_ptr<Primitive>> *prims_;
	};
}
