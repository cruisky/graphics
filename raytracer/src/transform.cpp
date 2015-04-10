#include "transform.h"
#include "core/matrix.h"
#include "core/ray.h"
#include "rayhit.h"

namespace Cruisky{

	inline void Transform::Translate(const Vector3& translation){
		local_world_ = local_world_ * Matrix4x4::Translate(translation);
		world_local_ = Matrix4x4::Translate(-translation) * world_local_;
	}

	inline void Transform::Rotate(const Vector3& rotation){
		local_world_ = local_world_ * Matrix4x4::Rotate(rotation);
		world_local_ = Matrix4x4::Rotate(-rotation) * world_local_;
	}

	inline void Transform::Rotate(float x, float y, float z){
		local_world_ = local_world_ * Matrix4x4::Rotate(x, y, z);
		world_local_ = Matrix4x4::Rotate(-x, -y, -z) * world_local_;
	}

	inline void Transform::Rotate(float angle, const Vector3& axis){
		local_world_ = local_world_ * Matrix4x4::Rotate(angle, axis);
		world_local_ = Matrix4x4::Rotate(-angle, axis) * world_local_;
	}

	inline void Transform::Scale(const Vector3& scale){
		local_world_ = local_world_ * Matrix4x4::Rotate(scale);
		world_local_ = Matrix4x4::Rotate(Vector3(1.f / scale.x, 1.f / scale.y, 1.f / scale.z)) * world_local_;
	}

	inline Ray Transform::ToWorld(const Ray& ray) const{
		return Ray(
			Matrix4x4::TPoint(local_world_, ray.origin),
			Matrix4x4::TVector(local_world_, ray.dir),
			ray.t_min, ray.t_max);
	}

	inline RayHit Transform::ToWorld(const RayHit& hit) const{
		return RayHit(
			Matrix4x4::TPoint(local_world_, hit.point),
			Matrix4x4::TNormal(local_world_, hit.normal),
			hit.t);
	}

	inline Ray Transform::ToLocal(const Ray& ray) const{
		return Ray(
			Matrix4x4::TPoint(world_local_, ray.origin),
			Matrix4x4::TVector(world_local_, ray.dir),
			ray.t_min, ray.t_max);
	}

	inline RayHit Transform::ToLocal(const RayHit& hit) const{
		return RayHit(
			Matrix4x4::TPoint(world_local_, hit.point),
			Matrix4x4::TNormal(world_local_, hit.normal),
			hit.t);
	}
}