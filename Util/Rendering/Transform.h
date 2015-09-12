#pragma once
#include "RenderingFwd.h"
#include "Math/Matrix.h"
#include "Math/Ray.h"

namespace TX{
	namespace Rendering
	{
		class Transform {
		public:
			Transform(){}
			Transform(const Transform& tr) : position_(tr.position_), local_world_(tr.local_world_), world_local_(tr.world_local_){}
			~Transform(){}
			inline Transform& operator = (const Transform& tr){ 
				position_ = tr.position_;
				local_world_ = tr.local_world_;
				world_local_ = tr.world_local_;
			}
			inline Vector3 Right() const { return Matrix4x4::TVector(local_world_, Vector3::X); }
			inline Vector3 Up() const { return Matrix4x4::TVector(local_world_, Vector3::Y); }
			inline Vector3 Forward() const { return Matrix4x4::TVector(local_world_, -Vector3::Z); }
			inline const Matrix4x4& WorldToLocalMatrix() const { return world_local_; }
			inline const Matrix4x4& LocalToWorldMatrix() const { return local_world_; }
			inline const Vector3& Position() const { return position_; }

			Transform& SetPosition(const Vector3& position);
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

			// Transforms a local ray to world space and normalize it.
			inline void ToWorld(Ray& ray) const {
				ray.origin = Matrix4x4::TPoint(local_world_, ray.origin);
				ray.dir = Matrix4x4::TVector(local_world_, ray.dir).Normalize();
			}

			// Transforms a global ray to local space WITHOUT normalizing it (so that t_max is valid)
			inline void ToLocal(Ray& ray) const{
				ray.origin = Matrix4x4::TPoint(world_local_, ray.origin);
				ray.dir = Matrix4x4::TVector(world_local_, ray.dir);
			}

		private:
			Vector3 position_;
			Matrix4x4 local_world_;
			Matrix4x4 world_local_;
		};
	}
}