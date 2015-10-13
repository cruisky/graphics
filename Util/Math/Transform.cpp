#include "UtilStdAfx.h"
#include "Transform.h"

namespace TX{
	Transform& Transform::Translate(const Vec3& translation, Space space){
		switch (space) {
		case Space::LOCAL:
			data_.pos += data_.rot.Rotate(translation);
			break;
		case Space::WORLD:
			data_.pos += translation;
			break;
		}
		MarkDirty();
		return *this;
	}

	Transform& Transform::Rotate(const Quaternion& rot, Space space){
		switch (space) {
		case Space::LOCAL:
			data_.rot = Math::Normalize(data_.rot * rot);
			break;
		case Space::WORLD:
			data_.rot = Math::Normalize(rot * data_.rot);
			break;
		}
		MarkDirty();
		return *this;
	}

	Transform& Transform::Scale(const Vec3& scale){
		data_.scale = Math::Max(data_.scale * scale, Vec3(Math::MIN));
		MarkDirty();
		return *this;
	}

	void Transform::Update() const {
		if (dirty_) {
			// apply scale, rotation, translation in this order (by the right associativity)
			// so that translation isn't affected by scale or rotation
			local_world_ = Matrix4x4::Translate(data_.pos)
				* data_.rot.RotationMatrix4x4()
				* Matrix4x4::Scale(data_.scale);
			world_local_ = // local_world_.Inverse();
				Matrix4x4::Scale(Vec3::ONE / data_.scale)
				* data_.rot.Conjugate().RotationMatrix4x4()
				* Matrix4x4::Translate(-data_.pos);
			dirty_ = false;
		}
	}
}
