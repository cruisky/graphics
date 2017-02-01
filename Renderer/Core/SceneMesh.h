#pragma once

#include "TXBase/Shape/Mesh.h"
#include "TXBase/Math/Vector.h"
#include "TXBase/Math/BBox.h"

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
