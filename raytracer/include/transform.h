#pragma once

#include "fwddecl.h"
#include "core/matrix.h"

namespace Cruisky{
	namespace RayTracer
	{
		class Transform {
		public:
			Transform(){}
			~Transform(){}

			const Matrix4x4& GetWorldToLocal() const { return world_local_; }
			const Matrix4x4& GetLocalToWorld() const { return local_world_; }

			inline void Translate(const Vector3& translation);
			inline void Rotate(const Vector3& rotation);
			inline void Rotate(float x, float y, float z);
			inline void Rotate(float angle, const Vector3& axis);
			inline void Scale(const Vector3& scale);

			inline Ray		ToWorld(const Ray& ray) const;
			inline RayHit	ToWorld(const RayHit& hit) const;

			inline Ray		ToLocal(const Ray& ray) const;
			inline RayHit	ToLocal(const RayHit& hit) const;

		private:
			Matrix4x4 local_world_;
			Matrix4x4 world_local_;
		};
	}
}