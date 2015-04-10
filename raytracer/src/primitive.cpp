#include "primitive.h"
#include "core/matrix.h"
#include "core/ray.h"
#include "shape.h"
#include "rayhit.h"
#include "intersection.h"

namespace Cruisky{

	namespace RayTracer
	{
		bool GeometricPrimitive::Intersect(const Ray& ray, Intersection& intxn){
			const Matrix4x4& world_to_local = transform.GetWorldToLocal();
			const Matrix4x4& local_to_world = transform.GetLocalToWorld();

			// transform the ray into local space
			Ray localRay = transform.ToLocal(ray);
			RayHit hit(ray.t_max);	// set to max length of the ray

			if (shape->Intersect(localRay, hit)) return false;
			intxn.hit = hit;
			intxn.material = material;
			return true;
		}

		bool GeometricPrimitive::Intersect(const Ray& ray){
			const Matrix4x4& world_to_local = transform.GetWorldToLocal();
			const Matrix4x4& local_to_world = transform.GetLocalToWorld();
			Ray localRay = transform.ToLocal(ray);
			return shape->Intersect(localRay);
		}
	}

}