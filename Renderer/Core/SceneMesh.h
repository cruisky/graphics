#pragma once

#include "txbase/shape/mesh.h"
#include "txbase/math/vector.h"
#include "txbase/math/bbox.h"

namespace TX {
	class LocalGeo;

	class SceneMesh : public Mesh {
	public:
		SceneMesh() {}
		SceneMesh(const Mesh& ot) :
			Mesh(ot) {
		}
		//inline SceneMesh& operator = (const SceneMesh& ot) {
		//	Mesh::operator=(ot);
		//	return *this;
		//}
		//~SceneMesh() {}

		void PostIntersect(LocalGeo& geo) const;
	};
}
