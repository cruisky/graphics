#pragma once
#include "fwddecl.h"

#include "SceneObject.h"

namespace Cruisky
{
	namespace RayTracer{
		class Camera : public SceneObject {
		public:
			Camera(int width, int height, float fov = 90.f, float near = 0.1f, float far = 1000.f, bool is_ortho = false);

			void GenerateRay(Ray *out, const Sample& sample) const;

			inline int Width() const { return width_; }
			inline int Height() const { return height_; }
			// Returns fov if in perspective mode, and size if in orthographic mode
			inline float FOV() const { return fov_; }
			inline float NearClipPlane() const { return clip_near_; }
			inline float FarClipPlane() const { return clip_far_; }
			inline bool IsOrtho() const { return is_ortho_; }

			// Transforms point from screen to world (on the near plane)
			Vector3 ScreenToWorldPoint(const Vector3& pix) const;
			// Transforms point from world to screen.
			// z component is in canonical view volume unit
			Vector3 WorldToScreenPoint(const Vector3& point) const;

			Camera& Resize(int w, int h);
			Camera& SetFOV(float fov);
			Camera& SetNearClipPlane(float near);
			Camera& SetFarClipPlane(float far);
			Camera& SetOrtho(bool ortho);
		
		private:
			// Updates the camera-screen matrices
			inline void UpdateMainMatrix();
			// Updates the camera-viewport matrices, should also call UpdateMainMatrix() after calling this
			inline void UpdateProjection();
			// Updates the viewport-screen matrices, should also call UpdateMainMatrix() after calling this
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
			cam_screen_ = cam_viewport_ * viewport_screen_;
			screen_cam_ = screen_viewport_ * viewport_cam_;
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
}