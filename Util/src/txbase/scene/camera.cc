#include "txbase/stdafx.h"
#include "camera.h"
#include "txbase/math/sample.h"

namespace TX{
	Camera::Camera(int res_x, int res_y, float fov, float near, float far, bool ortho) :
		fov_(fov), clip_near_(near), clip_far_(far), is_ortho_(ortho) {
		Resize(res_x, res_y);
	}

	void Camera::GenerateRay(Ray *out, float screenX, float screenY) const {
		out->Reset();
		// construct the ray in camera space
		out->origin = Vec3::ZERO;
		// direction to the point on near plane in camera space
		out->dir = Matrix4x4::TPoint(screen_cam_, Vec3(screenX, screenY, -1.f));
		// transforms the ray to the world space
		transform.ToWorld(*out);
	}


	Vec3 Camera::ScreenToWorldPoint(const Vec3& pix) const{
		return Matrix4x4::TPoint(transform.LocalToWorldMatrix(),
			Matrix4x4::TPoint(screen_cam_, pix));
	}
	Vec3 Camera::WorldToScreenPoint(const Vec3& point) const {
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
