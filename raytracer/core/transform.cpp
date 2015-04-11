#include "transform.h"

namespace Cruisky{
	namespace RayTracer {
		Transform& Transform::Translate(const Vector3& translation){
			Matrix4x4 tr = Matrix4x4::Translate(translation);
			position_ = Matrix4x4::TPoint(tr, position_);
			local_world_ = local_world_ * tr;
			world_local_ = Matrix4x4::Translate(-translation) * world_local_;
			return *this;
		}

		Transform& Transform::Rotate(const Vector3& rotation){
			Matrix4x4 mrot = Matrix4x4::Rotate(rotation);
			local_world_ = local_world_ * mrot;
			world_local_ = mrot.Transpose() * world_local_;
			return *this;
		}

		Transform& Transform::Rotate(float angle, const Vector3& axis){
			Matrix4x4 mrot = Matrix4x4::Rotate(angle, axis);
			local_world_ = local_world_ * mrot;
			world_local_ = mrot.Transpose() * world_local_;
			return *this;
		}

		Transform& Transform::LookAt(const Vector3& dir){
			// find arbitrary vector that's perp to dir
			return LookAt(dir, Vector3(-1.f, -1.f, dir.x + dir.y));
		}

		Transform& Transform::LookAt(const Vector3& dir, const Vector3& up){
			local_world_ = Matrix4x4::LookAt(position_, position_ + dir, up);
			world_local_ = local_world_.InverseRotation();
			return *this;
		}

		Transform& Transform::Scale(const Vector3& scale){
			local_world_ = local_world_ * Matrix4x4::Rotate(scale);
			world_local_ = Matrix4x4::Rotate(Vector3(1.f / scale.x, 1.f / scale.y, 1.f / scale.z)) * world_local_;
			return *this;
		}

	}
}