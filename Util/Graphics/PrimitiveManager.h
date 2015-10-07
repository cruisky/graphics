#pragma once
#include "Util.h"
#include <memory>
namespace TX
{
	class PrimitiveManager {
	public:
		PrimitiveManager(std::vector<std::shared_ptr<Primitive>> *prims){
			prims_ = prims;
		}
		virtual ~PrimitiveManager(){}

		virtual void Construct(){}

		virtual bool Intersect(const Ray& ray, Intersection& intxn) const;
		virtual bool Occlude(const Ray& ray) const;
		
	public:
		std::vector<std::shared_ptr<Primitive>> *prims_;
	};
}