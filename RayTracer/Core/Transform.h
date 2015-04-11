#pragma once

#include "fwddecl.h"
#include "Matrix.h"
#include "Ray.h"
#include "RayHit.h"

namespace Cruisky{
	namespace RayTracer
	{
		class Transform {
		public:
			Transform(){}
			Transform(Vector3 position, Vector3 rotation = Vector3::ZERO, Vector3 scale = Vector3::ONE){
				if(position != Vector3::ZERO) Translate(position);
				if(rotation != Vector3::ZERO) Rotate(rotation);
				if(scale != Vector3::ONE) Scale(scale);
			}
			~Transform(){}

			const Vector3& GetPosition() const { return position_; }
			const Matrix4x4& GetWorldToLocal() const { return world_local_; }
			const Matrix4x4& GetLocalToWorld() const { return local_world_; }

			Transform& Translate(const Vector3& translation);
			Transform& Rotate(const Vector3& rotation);
			Transform& Rotate(float angle, const Vector3& axis);
			Transform& Scale(const Vector3& scale);
			Transform& LookAt(const Vector3& dir);
			Transform& LookAt(const Vector3& dir, const Vector3& up);
			//Transform& LookAt(const Vector3& pt_eye, const Vector3& pt_target, const Vector3& up);

			inline Transform& Translate(float x, float y, float z){ return Translate(Vector3(x, y, z)); }
			inline Transform& Rotate(float x, float y, float z){ return Rotate(Vector3(x, y, z)); }
			inline Transform& Scale(float x, float y, float z){ return Scale(Vector3(x, y, z)); }

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
			Vector3 position_;
			Matrix4x4 local_world_;
			Matrix4x4 world_local_;
		};
	}
}