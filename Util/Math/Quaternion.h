#pragma once
#include "Vector.h"
#include "Matrix.h"

namespace TX
{
	class Quaternion {
	public:
		union {
			struct { Vec4 q; };
			struct {
				union {
					struct { Vec3 xyz; };
					struct { float x, y, z; };
				};
				float w;
			};
		};
		static constexpr float EPSILON = 1e-6f;
		static Quaternion IDENTITY;
	public:
		Quaternion() : q(Math::ZERO, Math::ZERO, Math::ZERO, Math::ONE){}
		Quaternion(const Quaternion& ot) : q(ot.q) {}
		Quaternion(float x, float y, float z, float w) : q(x, y, z, w) {}
		Quaternion(const Vec3& v, float w) : xyz(v), w(w) {}
		Quaternion& operator = (const Quaternion& ot) { q = ot.q; return *this; }
		explicit Quaternion(const Vec4& v) : q(v){}

		inline bool operator == (const Quaternion& ot) const { return q == ot.q; }
		inline bool operator != (const Quaternion& ot) const { return q != ot.q; }
		inline Quaternion operator + () const { return Quaternion(q); }
		inline Quaternion operator - () const { return Quaternion(-q); }
		inline Quaternion operator + (const Quaternion& rhs) const { return Quaternion(q + rhs.q); }
		inline Quaternion operator - (const Quaternion& rhs) const { return Quaternion(q - rhs.q); }
		inline Quaternion operator * (float s) const { return Quaternion(q * s); }
		inline Quaternion operator * (const Quaternion& rhs) const {
			return Quaternion(
				w*rhs.x + x*rhs.w + y*rhs.z - z*rhs.y,
				w*rhs.y + y*rhs.w + z*rhs.x - x*rhs.z,
				w*rhs.z + z*rhs.w + x*rhs.y - y*rhs.x,
				w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z);
		}
		inline Quaternion operator / (float s) const { return Quaternion(q / s); }
		inline Quaternion& operator += (const Quaternion& rhs) { q += rhs.q; return *this; }
		inline Quaternion& operator -= (const Quaternion& rhs) { q -= rhs.q; return *this; }
		inline Quaternion& operator *= (float s) { q *= s; return *this; }
		inline Quaternion& operator *= (const Quaternion& rhs) { return this->operator=(this->operator*(rhs)); }
		inline Quaternion& operator /= (float s) { q /= s; return *this; }

		inline float Norm() const { return Math::Length(q); }
		inline Quaternion Conjugate() const { return Quaternion(-xyz, w); }
		inline Quaternion Inverse() const { return Conjugate() / Norm(); }

		inline Vec3 Rotate(const Vec3& v) const {
			//return (((*this) * Quaternion(v, 0.f)) * Conjugate()).xyz;
			Vec4 tmp(
				w * v.x + y * v.z - z * v.y,
				w * v.y + z * v.x - x * v.z,
				w * v.z + x * v.y - y * v.x,
				-x * v.x - y * v.y - z * v.z);
			return Vec3(
				-tmp.w * x + tmp.x * w - tmp.y * z + tmp.z * y,
				-tmp.w * y + tmp.y * w - tmp.z * x + tmp.x * z,
				-tmp.w * z + tmp.z * w - tmp.x * y + tmp.y * x);
		}

		/*
		 * Return Euler angles in ZYX order.
		 */
		inline Vec3 Euler() const {
			Vec3 euler;
			euler.y = Math::Asin(2.f * (w*y - x*z));
			Vec4 sqrQ = q * q;
			euler.x = Math::Atan2(2.f * (w*x + y*z), sqrQ.w - sqrQ.x - sqrQ.y + sqrQ.z);
			euler.z = Math::Atan2(2.f * (w*z + x*y), sqrQ.x - sqrQ.y - sqrQ.z + sqrQ.w);
			return euler;
		}

		/*
		* Computes the rotation matrix represented by a UNIT quaternion.
		*/
		inline Matrix4x4 RotationMatrix4x4() const {
			float twox = 2 * x, twoy = 2 * y, twoz = 2 * z;
			float twoxx = twox*x, twoyy = twoy*y, twozz = twoz*z;
			float twoxy = twox*y, twoyz = twoy*z, twozw = twoz*w, twoxz = twox*z, twoxw = twox*w, twoyw = twoy*w;
			return Matrix4x4(
				1 - (twoyy + twozz), (twoxy - twozw), (twoxz + twoyw), 0.f,
				(twoxy + twozw), 1 - (twoxx + twozz), (twoyz - twoxw), 0.f,
				(twoxz - twoyw), (twoyz + twoxw), 1 - (twoxx + twoyy), 0.f,
				0.f, 0.f, 0.f, 1.f);
		}

		/*
		 * Computes the quaternion from euler angle rotation in ZYX order.
		 */
		static inline Quaternion Euler(const Vec3& euler);
		/*
		* Computes the quaternion from euler angle rotation in ZYX order.
		*/
		static inline Quaternion Euler(float x, float y, float z);
		static inline Quaternion RotationMatrix(const Matrix4x4& mat);
		static inline Quaternion AngleAxis(float angle, const Vec3& nAxis);
		static inline Quaternion FromToRotation(const Vec3& nFromDir, const Vec3& nToDir);
		static inline Quaternion LookRotation(const Vec3& nForward, const Vec3& nUpHint);
	};

	namespace Math {
		inline Quaternion Abs(const Quaternion& q) { return Quaternion(Abs(q.q)); }
		inline float Dot(const Quaternion& q1, const Quaternion q2) { return Dot(q1.q, q2.q); }
		inline float AbsDot(const Quaternion& q1, const Quaternion q2) { return Abs(Dot(q1.q, q2.q)); }
		inline Quaternion Normalize(const Quaternion& q) { return Quaternion(Normalize(q.q)); }
		inline Quaternion Slerp(float t, const Quaternion& q1, const Quaternion& q2) {
			float omega = Acos(Clamp(Dot(q1, q2), -1.f, 1.f));
			if (Abs(omega) < EPSILON)
				omega = EPSILON;
			float som_rcp = 1.f / Sin(omega);
			float st1 = Sin((1.f - t) * omega) * som_rcp;
			float st2 = Sin(t * omega) * som_rcp;
			return Quaternion(
				q1.x * st1 + q2.x * st2,
				q1.y * st1 + q2.y * st2,
				q1.z * st1 + q2.z * st2,
				q1.w * st1 + q2.w * st2);
		}
	}

	inline Quaternion Quaternion::Euler(const Vec3& euler) {
		return Euler(euler.x, euler.y, euler.z);
	}
	inline Quaternion Quaternion::Euler(float x, float y, float z) {
		Vec3 h(x * 0.5f, y * 0.5f, z * 0.5f);
		float cz = Math::Cos(h.z);
		float cy = Math::Cos(h.y);
		float cx = Math::Cos(h.x);
		float sz = Math::Sin(h.z);
		float sy = Math::Sin(h.y);
		float sx = Math::Sin(h.x);
		return Quaternion(
			sx*cy*cz - cx*sy*sz,
			cx*sy*cz + sx*cy*sz,
			cx*cy*sz - sx*sy*cz,
			cx*cy*cz + sx*sy*sz);

	}
	inline Quaternion Quaternion::RotationMatrix(const Matrix4x4& m) {
		// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/forum.htm
		float trace = m[0][0] + m[1][1] + m[2][2]; // I removed + 1.0f; see discussion with Ethan
		if (trace > 0) {// I changed M_EPSILON to 0
			float s = 0.5f / Math::Sqrt(trace + 1.0f);
			return Quaternion(
				(m[2][1] - m[1][2]) * s,
				(m[0][2] - m[2][0]) * s,
				(m[1][0] - m[0][1]) * s,
				0.25f / s);
		}
		else {
			if (m[0][0] > m[1][1] && m[0][0] > m[2][2]) {
				float s = 2.0f * Math::Sqrt(1.f + m[0][0] - m[1][1] - m[2][2]);
				return Quaternion(
					0.25f * s,
					(m[0][1] + m[1][0]) / s,
					(m[0][2] + m[2][0]) / s,
					(m[2][1] - m[1][2]) / s);
			}
			else if (m[1][1] > m[2][2]) {
				float s = 2.0f * Math::Sqrt(1.f + m[1][1] - m[0][0] - m[2][2]);
				return Quaternion(
					(m[0][1] + m[1][0]) / s,
					0.25f * s,
					(m[1][2] + m[2][1]) / s,
					(m[0][2] - m[2][0]) / s);
			}
			else {
				float s = 2.0f * Math::Sqrt(1.f + m[2][2] - m[0][0] - m[1][1]);
				return Quaternion(
					(m[0][2] + m[2][0]) / s,
					(m[1][2] + m[2][1]) / s,
					0.25f * s,
					(m[1][0] - m[0][1]) / s);
			}
		}
	}
	inline Quaternion Quaternion::AngleAxis(float rad, const Vec3& nAxis) {
		float halfAngle = rad * 0.5f;
		return Quaternion(nAxis * Math::Sin(halfAngle), Math::Cos(halfAngle));
	}

	/*
	 * Create a rotation to the new 'forward' direction.
	 * NOTE: Matrix4x4::LookAt is used to generate the view matrix, which transforms from world to local
	 * This one does the opposite rotation.
	 */
	inline Quaternion Quaternion::LookRotation(const Vec3& nForward, const Vec3& nUpHint = Vec3::UP) {
		return RotationMatrix(Matrix4x4::LookAt(Vec3::ZERO, nForward, nUpHint).Transpose());
	}

	inline Quaternion Quaternion::FromToRotation(const Vec3& nFrom, const Vec3& nTo) {
		// http://lolengine.net/blog/2014/02/24/quaternion-from-two-vectors-final
		float real = 1.f + Math::Dot(nFrom, nTo);
		Vec3 imaginary;
		if (real < EPSILON) {
			// opposite directions
			real = 0.f;
			imaginary = Math::Abs(nFrom.x) > Math::Abs(nFrom.z) ?
				Vec3(-nFrom.y, nFrom.x, 0.f) : Vec3(0.f, -nFrom.z, nFrom.y);
		}
		else {
			imaginary = Math::Cross(nFrom, nTo);
		}
		return Math::Normalize(Quaternion(imaginary, real));
	}

	inline Quaternion operator * (float s, const Quaternion& q) { return q * s; }
	inline std::ostream& operator << (std::ostream& os, const Quaternion& q) {
		return os << "(" << q.x << "i + " << q.y << "j + " << q.z << "k + " << q.w << ")";
	}

}
