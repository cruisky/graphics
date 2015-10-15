#pragma once
#include "Util.h"
#include "Math/Transform.h"

namespace TX {
	class Transform;
	class CameraController {
	private:
		Transform& target_;
	public:
		CameraController(Transform& target) : target_(target){}
		~CameraController(){}

		/// <summary>
		/// Take a 2d input and rotate the transform.
		/// </summary>
		inline void Turn(const Vec2& angularVelocity) {
			target_.Rotate(Quaternion::Euler(-angularVelocity.y, angularVelocity.x, 0.f), Space::LOCAL);
		}

		/// <summary>
		/// Take a 2d input and move horizontally.
		/// </summary>
		inline void HorizontalMove(float deltaTime, const Vec2& velocity) {
			auto translation = Math::Lerp(deltaTime, Vec2::ZERO, velocity);
			target_.Translate(translation.x, 0.f, -translation.y, Space::LOCAL);
		}

		/// <summary>
		/// Rotate the transform around the forward axis.
		/// </summary>
		inline void Roll(float deltaTime, float angularVelocity) {
			auto rotation = Math::Slerp(
				deltaTime * angularVelocity,
				Quaternion::IDENTITY,
				Quaternion::AngleAxis(Math::PI, -Vec3::Z)
				);
			target_.Rotate(rotation, Space::LOCAL);
		}

		/// <summary>
		/// Take a 1d input and move vertically.
		/// </summary>
		inline void VerticalMove(float deltaTime, float velocity) {
			auto translation = Math::Lerp(deltaTime, 0, velocity);
			target_.Translate(0.f, translation, 0.f, Space::WORLD);
		}
	};
}
