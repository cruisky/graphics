#pragma once
#include "Util.h"
#include "SceneObject.h"

namespace TX
{
	class Camera {
	public:
		Camera(int width, int height, float fov = 90.f, float near = 0.1f, float far = 1000.f, bool is_ortho = false);

		void GenerateRay(Ray *out, float screenX, float screenY) const;

		inline int Width() const { return width_; }
		inline int Height() const { return height_; }
		/// <summary>
		/// Returns fov if in perspective mode, and size if in orthographic mode
		/// </summary>
		inline float FOV() const { return fov_; }
		inline float NearClipPlane() const { return clip_near_; }
		inline float FarClipPlane() const { return clip_far_; }
		inline bool IsOrtho() const { return is_ortho_; }

		/// <summary>
		/// Transforms point from screen to world (on the near plane)
		/// </summary>
		Vec3 ScreenToWorldPoint(const Vec3& pix) const;
		/// <summary>
		/// ransforms point from world to screen.
		/// z component is in canonical view volume unit
		/// </summary>
		Vec3 WorldToScreenPoint(const Vec3& point) const;

		Camera& Resize(int w, int h);
		Camera& SetFOV(float fov);
		Camera& SetNearClipPlane(float near);
		Camera& SetFarClipPlane(float far);
		Camera& SetOrtho(bool ortho);
		inline const Matrix4x4& CameraToScreen() const { return cam_screen_; }
		inline const Matrix4x4& ScreenToCamera() const { return screen_cam_; }
		inline const Matrix4x4& ScreenToViewport() const { return screen_viewport_; }
		inline const Matrix4x4& ViewportToScreen() const { return viewport_screen_; }
		inline const Matrix4x4& CameraToViewport() const { return cam_viewport_; }
		inline const Matrix4x4& ViewportToCamera() const { return viewport_cam_; }

	public:
		Transform transform;

	private:
		/// <summary>
		/// Updates the camera-screen matrices
		/// </summary>
		inline void UpdateMainMatrix();
		/// <summary>
		/// Updates the camera-viewport matrices, should also call UpdateMainMatrix() after calling this
		/// </summary>
		inline void UpdateProjection();
		/// <summary>
		/// Updates the viewport-screen matrices, should also call UpdateMainMatrix() after calling this
		/// </summary>
		inline void UpdateViewport();

	private:
		bool is_ortho_;
		int width_, height_;
		float clip_near_, clip_far_;
		float fov_;

		Matrix4x4 cam_viewport_;
		Matrix4x4 viewport_screen_;
		Matrix4x4 screen_viewport_;
		Matrix4x4 viewport_cam_;

		Matrix4x4 cam_screen_;		// cam_viewport_ * viewport_screen_
		Matrix4x4 screen_cam_;		// screen_viewport_ * viewport_cam_
	};


	void Camera::UpdateMainMatrix(){
		cam_screen_ = viewport_screen_ * cam_viewport_;
		screen_cam_ = viewport_cam_ * screen_viewport_;
	}

	void Camera::UpdateProjection(){
		float ratio = float(width_) / height_;
		cam_viewport_ = is_ortho_ ? Matrix4x4::Orthographic(ratio, fov_, clip_near_, clip_far_)
			: Matrix4x4::Perspective(ratio, fov_, clip_near_, clip_far_);
		viewport_cam_ = cam_viewport_.Inverse();
	}

	void Camera::UpdateViewport(){
		viewport_screen_ = Matrix4x4::Viewport(float(width_), float(height_));
		screen_viewport_ = viewport_screen_.Inverse();
	}
}
