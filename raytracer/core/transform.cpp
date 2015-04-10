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
			local_world_ = local_world_ * Matrix4x4::Rotate(rotation);
			world_local_ = Matrix4x4::Rotate(-rotation) * world_local_;
			return *this;
		}

		Transform& Transform::Rotate(float angle, const Vector3& axis){
			local_world_ = local_world_ * Matrix4x4::Rotate(angle, axis);
			world_local_ = Matrix4x4::Rotate(-angle, axis) * world_local_;
			return *this;
		}

		Transform& Transform::Scale(const Vector3& scale){
			local_world_ = local_world_ * Matrix4x4::Rotate(scale);
			world_local_ = Matrix4x4::Rotate(Vector3(1.f / scale.x, 1.f / scale.y, 1.f / scale.z)) * world_local_;
			return *this;
		}

	}
}