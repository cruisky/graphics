#include "primitive.h"
#include "core/matrix.h"

namespace Cruisky{

	// ------------------------------------
	// Primitive
	// ------------------------------------

	inline void Primitive::Translate(const Vector3& translation){
		local_world_ = local_world_ * Matrix4x4::Translate(translation);
		world_local_ = Matrix4x4::Translate(-translation) * world_local_;
	}
	inline void Primitive::Rotate(const Vector3& rotation){
		local_world_ = local_world_ * Matrix4x4::Rotate(rotation);
		world_local_ = Matrix4x4::Rotate(-rotation) * world_local_;
	}
	inline void Primitive::Rotate(float x, float y, float z){
		local_world_ = local_world_ * Matrix4x4::Rotate(x, y, z);
		world_local_ = Matrix4x4::Rotate(-x, -y, -z) * world_local_;
	}
	inline void Primitive::Rotate(float angle, const Vector3& axis){
		local_world_ = local_world_ * Matrix4x4::Rotate(angle, axis);
		world_local_ = Matrix4x4::Rotate(-angle, axis) * world_local_;
	}
	inline void Primitive::Scale(const Vector3& scale){
		local_world_ = local_world_ * Matrix4x4::Rotate(scale);
		world_local_ = Matrix4x4::Rotate(Vector3(1.f/scale.x, 1.f/scale.y, 1.f/scale.z)) * world_local_;
	}

	// ------------------------------------
	// UnitSphere
	// ------------------------------------

	bool UnitSphere::Intersect(const Ray& ray, Intersection& intxn) {
		// transform the ray into local space
		Vector3 e = Matrix4x4::TPoint(world_local_, ray.origin); 
		Vector3 d = Matrix4x4::TVector(world_local_, ray.dir);
		float e2 = LengthSqr(e);
		float d2 = LengthSqr(d);
		float de = Dot(d, e);
		float delta_quarter = de * de - d2 * (e2 - 1);
		float t = -(de + Math::Sqrt(delta_quarter)) / d2;
		// proceed only if intersection occurred and is closer than before
		if (delta_quarter > 0 && t > 0 && (!intxn.exist || intxn.t > t)){
			Vector3 point = e + t * d;
			intxn.exist = true;
			intxn.t = t;
			intxn.point = Matrix4x4::TPoint(local_world_, point);
			intxn.normal = Matrix4x4::TNormal(world_local_, point).Normalize();
			return true;
		}
		return false;
	}

	// ------------------------------------
	// UnitPlane
	// ------------------------------------

	bool UnitPlane::Intersect(const Ray& ray, Intersection& intxn) {
		// transform the ray into local space
		Vector3 e = Matrix4x4::TPoint(world_local_, ray.origin);
		Vector3 d = Matrix4x4::TVector(world_local_, ray.dir);
		float t = -e.z / d.z;
		Vector3 point = e + t * d;
		if (t > 0 && (!intxn.exist || intxn.t > t) &&
			Math::InBounds(point.x, -0.5f, 0.5f) &&
			Math::InBounds(point.y, -0.5f, 0.5f)){
			intxn.exist = true;
			intxn.t = t;
			intxn.point = Matrix4x4::TPoint(local_world_, point);
			intxn.normal = Matrix4x4::TNormal(world_local_, Vector3(0, 0, 1)).Normalize();
			return true;
		}
		return false;
	}
}