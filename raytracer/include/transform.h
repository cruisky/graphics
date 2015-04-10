#pragma once

#include "fwddecl.h"
#include "core/matrix.h"
#include "core/ray.h"
#include "rayhit.h"

namespace Cruisky{
	namespace RayTracer
	{
		class Transform {
		public:
			Transform(){}
			~Transform(){}

			const Matrix4x4& GetWorldToLocal() const { return world_local_; }
			const Matrix4x4& GetLocalToWorld() const { return local_world_; }

			inline void Translate(const Vector3& translation){
				local_world_ = local_world_ * Matrix4x4::Translate(translation);
				world_local_ = Matrix4x4::Translate(-translation) * world_local_;
			}

			inline void Rotate(const Vector3& rotation){
				local_world_ = local_world_ * Matrix4x4::Rotate(rotation);
				world_local_ = Matrix4x4::Rotate(-rotation) * world_local_;
			}

			inline void Rotate(float x, float y, float z){
				local_world_ = local_world_ * Matrix4x4::Rotate(x, y, z);
				world_local_ = Matrix4x4::Rotate(-x, -y, -z) * world_local_;
			}

			inline void Rotate(float angle, const Vector3& axis){
				local_world_ = local_world_ * Matrix4x4::Rotate(angle, axis);
				world_local_ = Matrix4x4::Rotate(-angle, axis) * world_local_;
			}

			inline void Scale(const Vector3& scale){
				local_world_ = local_world_ * Matrix4x4::Rotate(scale);
				world_local_ = Matrix4x4::Rotate(Vector3(1.f / scale.x, 1.f / scale.y, 1.f / scale.z)) * world_local_;
			}

			inline Ray ToWorld(const Ray& ray) const{
				return Ray(
					Matrix4x4::TPoint(local_world_, ray.origin),
					Matrix4x4::TVector(local_world_, ray.dir).Normalize(),
					ray.t_min, ray.t_max);
			}

			inline RayHit ToWorld(const RayHit& hit) const{
				return RayHit(
					Matrix4x4::TPoint(local_world_, hit.point),
					Matrix4x4::TNormal(local_world_, hit.normal).Normalize());
			}

			inline Ray ToLocal(const Ray& ray) const{
				return Ray(
					Matrix4x4::TPoint(world_local_, ray.origin),
					Matrix4x4::TVector(world_local_, ray.dir).Normalize(),
					ray.t_min, ray.t_max);
			}

			inline RayHit ToLocal(const RayHit& hit) const{
				return RayHit(
					Matrix4x4::TPoint(world_local_, hit.point),
					Matrix4x4::TNormal(world_local_, hit.normal).Normalize());
			}

		private:
			Matrix4x4 local_world_;
			Matrix4x4 world_local_;
		};
	}
}