#include "UtilStdAfx.h"
#include "Primitive.h"
#include "Shape/Shape.h"
#include "Intersection.h"

namespace TX {
	Primitive& Primitive::Bake() {
		mesh->ApplyTransform(transform);
		transform = Transform();
		if (areaLight) {
			meshSampler = std::make_unique<MeshSampler>(mesh);
		}
		return *this;
	}

	void Primitive::PostIntersect(const Ray& ray, LocalGeo& geom) const {
		geom.point = ray.End();
		geom.bsdf = GetBSDF();
		mesh->PostIntersect(geom);
	}
}
