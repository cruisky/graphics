#pragma once

#include "core/matrix.h"
#include "core/ray.h"
#include "intersection.h"

namespace Cruisky {
	class Primitive{
	public:
		Primitive(){};
		~Primitive(){};

		inline void Translate(const Vector3& translation);
		inline void Rotate(const Vector3& rotation);
		inline void Rotate(float x, float y, float z);
		inline void Rotate(float angle, const Vector3& axis);
		inline void Scale(const Vector3& scale);

		virtual bool Intersect(const Ray& ray, Intersection& intersection) = 0;
	protected:
		Matrix4x4 local_world_;
		Matrix4x4 world_local_;
	};


	class UnitSphere : protected Primitive{
	public:
		UnitSphere(){}
		bool Intersect(const Ray& ray, Intersection& intersection);
	};

	class UnitPlane : protected Primitive{
	public:
		UnitPlane(){}
		bool Intersect(const Ray& ray, Intersection& intersection);
	};
}