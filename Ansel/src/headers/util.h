#pragma once

#include "Def.h"

#include <math.h>

namespace Ansel
{
	// <------------------>
	// <---  RECTANGLE --->
	// <------------------>

	template<typename T>
	struct ANSEL_API rect {
		T x, y, w, h;
	};
	
	/*! Rectangle with floats. */
	ANSEL_API typedef rect<float> FloatRect;
	/*! Rectangle with integers. */
	ANSEL_API typedef rect<int>   IntRect;
	/*! Rectangle with unsigned integers. */
	ANSEL_API typedef rect<unsigned int> uIntRect;


	// <------------------>
	// <---  MATRICES  --->
	// <------------------>

	struct ANSEL_API mat4x4 {
		float m[4][4] = { 0 };

		/**
		  * Sets matrix to an identity matrix. (modifies only center
		  * four.
		  */
		void setIdentity() {
			m[0][0] = 1.f;
			m[1][1] = 1.f;
			m[2][2] = 1.f;
			m[3][3] = 1.f;
		}

		/**
		  * Makes the matrix into a translation matrix for (x, y, z)
		  * @param x x-coordinate of the translation
		  * @param y y-coordinate of the translation
		  * @param z z-coordinate of the translation
		  */
		static mat4x4 makeTranslation(float x, float y, float z)
		{
			mat4x4 mat;

			mat.setIdentity();

			mat.m[3][0] = x;
			mat.m[3][1] = y;
			mat.m[3][2] = z;

			return mat;
		}
		
		/**
		  * Make the matrix into a rotation matrix.
		  * @param x Rotation relative to the x-axis (radians)
		  * @param y Rotation relative to the y-axis (radians)
		  * @param z Rotation relative to the z-axis (radians)
		  */
		static mat4x4 makeRotation(float x, float y, float z)
		{
			mat4x4 rot_x, rot_y, rot_z;

			rot_x.m[0][0] = 1.f;
			rot_x.m[1][1] = cosf(x);
			rot_x.m[1][2] = -sinf(x);
			rot_x.m[2][2] = cosf(x);
			rot_x.m[2][1] = sinf(x);
			rot_x.m[3][3] = 1.f;

			rot_y.m[0][0] = cosf(y);
			rot_y.m[0][2] = sinf(y);
			rot_y.m[1][1] = 1.f;
			rot_y.m[2][0] = -sinf(y);
			rot_y.m[2][2] = cosf(y);
			rot_y.m[3][3] = 1.f;

			return rot_x * rot_y;
		}
		
		/**
		  * Multiply two matrices together.
		  * @param mat Independant matrix
		  */
		mat4x4 operator*(mat4x4 mat) {
			mat4x4 nmat;

			for (int c = 0; c < 4; c++)
				for (int r = 0; r < 4; r++)
					nmat.m[c][r] = m[0][r] * mat.m[0][c] + m[1][r] * mat.m[1][c] + m[2][r] * mat.m[2][c] + m[3][r] * mat.m[3][c];

			return nmat;
		}
	};

	// <------------------>
	// <---  VERTICES  --->
	// <------------------>
	template<typename T>
	struct ANSEL_API vec2 {
		T x, y;

		// Casting Operator
		template<typename B>
		operator vec2<B>() const { return { (B)x, (B)y }; }
	};

	template<typename T>
	struct ANSEL_API vec3 {
		union
		{
			struct {
				T x, y, z;
			};

			struct {
				T r, g, b;
			};
		};

		// Division Operator
		template<typename B>
		vec3<T> operator/(const vec3<B> &v1) const {
			vec3<T> v;

			v.x = x / v1.x;
			v.y = y / v1.y;
			v.z = z / v1.z;

			return v;
		}

		template<typename B>
		vec3<T> operator/(const B &f) const {
			vec3<T> v;

			v.x = x / f;
			v.y = y / f;
			v.z = z / f;

			return v;
		}

		template<typename B>
		vec3<T> operator*(const B &f) const {
			vec3<T> v;

			v.x = x * f;
			v.y = y * f;
			v.z = z * f;

			return v;
		}

		// Subtraction Operator
		template<typename B>
		vec3<T> operator-(const vec3<B> &v1) const {
			vec3<T> v;

			v.x = x - v1.x;
			v.y = y - v1.y;
			v.z = z - v1.z;

			return v;
		}

		// Addition Operator
		template<typename B>
		vec3<T> operator+(const vec3<B> &v1) const {
			vec3<T> v;

			v.x = x + v1.x;
			v.y = y + v1.y;
			v.z = z + v1.z;

			return v;
		}

		// Plus Equals Operator
		template<typename B>
		void operator+=(const vec3<B> &v) {
			x += v.x;
			y += v.y;
			z += v.z;
		}

		// Comparison Operator
		template<typename B>
		bool operator==(const vec3<B> &v) {
			return (v.x == x && v.y == y && v.z == z);
		}

		// Casting Operator
		template<typename B>
		operator vec3<B>() const { return { (B)x, (B)y, (B)z }; }
	};

	template<typename T>
	struct ANSEL_API vec4	{
		union
		{
			struct {
				T x, y, z, w;
			};

			struct {
				T r, g, b, a;
			};
		};

		// Addition Operators
		template<typename B>
		vec4<B> operator+(const vec4<B> v) {
			return { x + v.x, y + v.y, z + v.z, w + v.w };
		}

		// Multplication Operators
		template<typename B>
		vec4<B> operator*(const float k) {
			return { x * k, y * k, z * k, w * k };
		}

		void operator*=(const float k) {
			x *= k;
			y *= k;
			z *= k;
			w *= k;
		}

		// Casting Operator
		template<typename B>
		operator vec4<B>() const { return { (B)x, (B)y, (B)z, (B)w }; }
	};

	// 2D Vector Type Definitions
	ANSEL_API typedef vec2<float>  vec2f;
	ANSEL_API typedef vec2<double> vec2d;
	ANSEL_API typedef vec2<int>    vec2i;
	ANSEL_API typedef vec2<unsigned int> vec2u;

	// 3D Vector Type Definitions
	ANSEL_API typedef vec3<float>  vec3f;
	ANSEL_API typedef vec3<double> vec3d;
	ANSEL_API typedef vec3<int>    vec3i;
	ANSEL_API typedef vec3<unsigned int> vec3u;

	// 4D Vector Type Definitions
	ANSEL_API typedef vec4<float>  vec4f;
	ANSEL_API typedef vec4<double> vec4d;
	ANSEL_API typedef vec4<int>    vec4i;
	ANSEL_API typedef vec4<unsigned int> vec4u;

	/**
	  * Get the dot product of two vectors with varying types.
	  * @param vec_a First vector
	  * @param vec_b Second vector
	  */
	template<typename T>
	inline float dotProduct(vec3<T> vec_a, vec3<T> vec_b) {
		return vec_a.x * vec_b.x + vec_a.y * vec_b.y + vec_a.z * vec_b.z;
	}

	/**
	  * Get the cross product of two vectors with varying types.
	  * @param vec_a First vector
	  * @param vec_b Second vector
	  */
	template<typename T>
	inline vec3<T> cross(vec3<T> vec_a, vec3<T> vec_b) {
		vec3<T> normal;

		normal.x = vec_a.y * vec_b.z - vec_a.z * vec_b.y;
		normal.y = vec_a.z * vec_b.x - vec_a.x * vec_b.z;
		normal.z = vec_a.x * vec_b.y - vec_a.y * vec_b.x;

		return normal;
	}

	/**
	  * Get the length of a vector.
	  * @param vec Vector to get length of
	  */
	template<typename T>
	inline float length(vec3<T> vec) {
		return sqrtf(dotProduct(vec, vec));
	}

	/**
	  * Normalize given vector.
	  * @param vec Vector to be normalized 
	  */
	template<typename T>
	inline vec3<T> normalize(vec3<T> vec) {
		return vec / length(vec);
	}
}
