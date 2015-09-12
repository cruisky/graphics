#pragma once
#include "RenderingFwd.h"
#include "SceneObject.h"
#include <memory>
#include "BSDF.h"		// shared_ptr
#include "Shape.h"		// shared_ptr

namespace TX {
	namespace Rendering
	{
		class Primitive : public SceneObject {
			// Allows an AreaLight to bind with this primitive at its creation.
			friend class AreaLight;
		public:
			Primitive(std::shared_ptr<const Shape> shape, std::shared_ptr<const BSDF> bsdf) : bsdf_(bsdf), shape_(shape), area_light_(nullptr){};
			virtual ~Primitive(){}

			// Checks if the global ray intersects this primitive, if so
			// fills dist and prim in the intersection, 
			// and modify the length (t_max) of the ray.
			// NOTE: The dir of the global ray should be normalized.
			bool Intersect(const Ray& ray, Intersection& intersection) const;
			// Extracts info for the LocalGeo.
			void PostIntersect(const Ray& ray, LocalGeo& geo) const;
			// Test if this object occludes the ray.
			bool Occlude(const Ray& ray) const;
			
			inline float Pdf(const Vector3& point) const { 
				return shape_->Pdf(Matrix4x4::TPoint(transform.WorldToLocalMatrix(), point));
			}

			inline float Pdf(const Vector3& point, const Vector3& dir) const {
				Ray localray;
				localray.Reset(point, dir);
				transform.ToLocal(localray);
				localray.dir.Normalize();		// normalize the local ray so that after intersection, t value will be the local distance
				return shape_->Pdf(localray);
			}

			inline void SamplePoint(const Sample *sample, Vector3 *point, Vector3 *normal/* = nullptr */) const {
				shape_->SamplePoint(sample, point, normal);
				*point = Matrix4x4::TPoint(transform.LocalToWorldMatrix(), *point);
				*normal = Matrix4x4::TNormal(transform.WorldToLocalMatrix(), *normal);	// use inverse when transforming normal
			}

			inline void SamplePoint(const Sample *sample, const Vector3& eye, Vector3 *point, Vector3 *normal) const {
				shape_->SamplePoint(sample, Matrix4x4::TPoint(transform.WorldToLocalMatrix(), eye), point, normal);
				*point = Matrix4x4::TPoint(transform.LocalToWorldMatrix(), *point);
				*normal = Matrix4x4::TNormal(transform.WorldToLocalMatrix(), *normal);	// use inverse when transforming normal
			}


			const BSDF* GetBSDF() const;
			const AreaLight* GetAreaLight() const;
		private:
			std::shared_ptr<const BSDF>			bsdf_;
			std::shared_ptr<const Shape>		shape_;
			const AreaLight *					area_light_;
		};
	}
}