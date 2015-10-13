#include "UtilStdAfx.h"
#include "Transform.h"

namespace TX{
	Transform& Transform::Translate(const Vec3& translation){
		data_.pos += translation;
		needUpdate = true;
		return *this;
	}

	Transform& Transform::Rotate(const Vec3& eulerAngles){
		data_.rot *= Quaternion::Euler(eulerAngles);
		needUpdate = true;
		return *this;
	}

	Transform& Transform::Rotate(float rad, const Vec3& axis){
		data_.rot *= Quaternion::AngleAxis(rad, axis);
		needUpdate = true;
		return *this;
	}

	Transform& Transform::LookAt(const Vec3& dir){
		// find arbitrary vector that's perp to dir
		return LookAt(dir, Vec3(-1.f, -1.f, dir.x + dir.y));
	}

	Transform& Transform::LookAt(const Vec3& dir, const Vec3& worldUp){
		data_.rot = Quaternion::LookRotation(dir, worldUp);
		needUpdate = true;
		return *this;
	}

	Transform& Transform::Scale(const Vec3& scale){
		data_.scale = Math::Max(data_.scale * scale, Vec3(Math::MIN));
		needUpdate = true;
		return *this;
	}

	void Transform::Update() const {
		if (needUpdate) {
			local_world_ = Matrix4x4::Scale(data_.scale)
				* data_.rot.RotationMatrix4x4()
				* Matrix4x4::Translate(data_.pos);
			world_local_ = Matrix4x4::Translate(-data_.pos)
				* data_.rot.Conjugate().RotationMatrix4x4()
				* Matrix4x4::Scale(Vec3::ONE / data_.scale);
		}
	}
}
