#pragma once
#include "Util.h"
#include "Math/Quaternion.h"
#include "Math/Matrix.h"
#include "Math/Ray.h"

namespace TX{
	enum class Space {
		LOCAL, WORLD
	};
	class Transform {
	private:
		struct TransformInfo {
			Vec3 pos;
			Quaternion rot;
			Vec3 scale;
			TransformInfo() : pos(Math::ZERO), rot(Quaternion::IDENTITY), scale(Math::ONE) {}
			TransformInfo(const TransformInfo& ot) : pos(ot.pos), rot(ot.rot), scale(ot.scale) {}
			inline TransformInfo& operator = (const TransformInfo& ot) { pos = ot.pos, rot = ot.rot, scale = ot.scale; return *this; }
		};
		TransformInfo data_;
		mutable bool dirty_;
		mutable Matrix4x4 local_world_;
		mutable Matrix4x4 world_local_;
	public:
		Transform() :
			data_(),
			local_world_(Matrix4x4::IDENTITY),
			world_local_(Matrix4x4::IDENTITY),
			dirty_(false) {}
		Transform(const Transform& ot) :
			data_(ot.data_),
			dirty_(true) {}
		~Transform(){}
		inline Transform& operator = (const Transform& tr){
			data_ = tr.data_;
			dirty_ = true;
		}
		inline Vec3 Right() const { return Matrix4x4::TVec(local_world_, Vec3::X); }
		inline Vec3 Up() const { return Matrix4x4::TVec(local_world_, Vec3::Y); }
		inline Vec3 Forward() const { return Matrix4x4::TVec(local_world_, -Vec3::Z); }
		inline const Matrix4x4& WorldToLocalMatrix() const { return world_local_; }
		inline const Matrix4x4& LocalToWorldMatrix() const { return local_world_; }

		inline const Vec3& GetPosition() const { return data_.pos; }
		inline const Quaternion& GetRotation() const { return data_.rot; }
		inline const Vec3& GetScale() const { return data_.scale; }
		inline Transform& SetPosition(const Vec3& position) { data_.pos = position; MarkDirty(); return *this; }
		inline Transform& SetRotation(const Quaternion& rotation) { data_.rot = rotation; MarkDirty(); return *this; }
		inline Transform& SetScale(const Vec3& scale) { data_.scale = scale; MarkDirty(); return *this; }

		Transform& Translate(const Vec3& translation, Space space = Space::LOCAL);
		Transform& Rotate(const Quaternion& rotation, Space space = Space::LOCAL);
		Transform& Scale(const Vec3& scale);
		inline Transform& Translate(float x, float y, float z, Space space = Space::LOCAL) { return Translate(Vec3(x, y, z), space); }
		inline Transform& Scale(float x, float y, float z) { return Scale(Vec3(x, y, z)); }
		inline Transform& LookAt(const Vec3& point, const Vec3& up) { return SetRotation(Quaternion::LookRotation(Math::Normalize(point - GetPosition()), up)); }

		// Transforms a local ray to world space and normalize it.
		inline void ToWorld(Ray& ray) const {
			ray.origin = Matrix4x4::TPoint(local_world_, ray.origin);
			ray.dir = Math::Normalize(Matrix4x4::TVec(local_world_, ray.dir));
		}
		// Transforms a global ray to local space WITHOUT normalizing it (so that t_max is valid)
		inline void ToLocal(Ray& ray) const {
			ray.origin = Matrix4x4::TPoint(world_local_, ray.origin);
			ray.dir = Matrix4x4::TVec(world_local_, ray.dir);
		}
		inline void MarkDirty() const { dirty_ = true; }
		void Update() const;
	};

	inline std::ostream& operator << (std::ostream& os, const Transform& tr) {
		os << "Position:\t" << tr.GetPosition() << std::endl;
		os << "Rotation:\t" << tr.GetRotation().Euler() << std::endl;
		os << "Scale:\t" << tr.GetScale() << std::endl;
		return os;
	}
}
