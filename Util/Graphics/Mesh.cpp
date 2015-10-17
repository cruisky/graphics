#include "UtilStdAfx.h"
#include "Mesh.h"
#include "Obj.h"

namespace TX {
	void Mesh::Clear() {
		vertices.clear();
		normals.clear();
		uv.clear();
		triangles.clear();
	}

	bool Mesh::Intersect(const Ray& localray) const {
		//TODO
		return false;
	}
	void Mesh::PostIntersect(const Ray& localray, LocalGeo& geo) const {
		//TODO
	}
	bool Mesh::Occlude(const Ray& localray) const {
		//TODO
		return false;
	}
	float Mesh::Area() const {
		//TODO
		return 1.f;
	}
	float Mesh::Pdf(const Ray& localwi) const {
		//TODO
		return 0.f;
	}
	void Mesh::SamplePoint(const Sample *sample, Vec3 *out, Vec3 *normal/* = nullptr*/) const {
		//TODO
	}
}
