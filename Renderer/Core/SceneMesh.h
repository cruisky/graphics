#pragma once
#include "Util.h"
#include "Shape/Mesh.h"
#include "Math/Vector.h"
#include "Math/BBox.h"

namespace TX {
	class SceneMesh : public Mesh {
	public:
		//SceneMesh() {}
		//SceneMesh(const SceneMesh& ot) :
		//	Mesh(ot) {
		//}
		//inline SceneMesh& operator = (const SceneMesh& ot) {
		//	Mesh::operator=(ot);
		//	return *this;
		//}
		//~SceneMesh() {}

		void PostIntersect(LocalGeo& geo) const;
	};
}
