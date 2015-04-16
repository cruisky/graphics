#include "stdafx.h"
#include "Camera.h"
#include "Sampler.h"

namespace Cruisky{
	namespace RayTracer{
		Camera::Camera(int res_x, int res_y, float fov, float near, float far, bool ortho) :
			fov_(fov), clip_near_(near), clip_far_(far), is_ortho_(ortho) {
			Resize(res_x, res_y);
		}

		void Camera::GenerateRay(Ray *out, const Sample& sample) const {
			out->Reset();
			// construct the ray in camera space
			out->origin = Vector3::ZERO;
			// direction to the point on near plane in camera space
			out->dir = Matrix4x4::TPoint(screen_cam_, Vector3(sample.u, sample.v, -1.f));
			// transforms the ray to the world space
			transform.ToWorld(*out);
		}

		Vector3 Camera::ScreenToWorldPoint(const Vector3& pix) const{
			return Matrix4x4::TPoint(transform.LocalToWorldMatrix(),
				Matrix4x4::TPoint(screen_cam_, pix));
		}
		Vector3 Camera::WorldToScreenPoint(const Vector3& point) const {
			return Matrix4x4::TPoint(cam_screen_,
				Matrix4x4::TPoint(transform.WorldToLocalMatrix(), point));
		}

		Camera& Camera::Resize(int w, int h) {
			width_ = w;
			height_ = h;
			UpdateProjection();
			UpdateViewport();
			UpdateMainMatrix();
			return *this;
		}

		Camera& Camera::SetFOV(float fov) {
			fov_ = fov;
			UpdateProjection();
			UpdateMainMatrix();
			return *this;
		}
		
		Camera& Camera::SetNearClipPlane(float near) {
			clip_near_ = near;
			UpdateProjection();
			UpdateMainMatrix();
			return *this;
		}

		Camera& Camera::SetFarClipPlane(float far) {
			clip_far_ = far;
			UpdateProjection();
			UpdateMainMatrix();
			return *this;
		}

		Camera& Camera::SetOrtho(bool ortho) {
			if (is_ortho_ ^ ortho){
				is_ortho_ = ortho;
				UpdateProjection();
				UpdateMainMatrix();
			}
			return *this;
		}
	}
}