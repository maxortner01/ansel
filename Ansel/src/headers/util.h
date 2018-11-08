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

	typedef rect<float> FloatRect;
	typedef rect<int>   IntRect;
	typedef rect<unsigned int> uIntRect;


	// <------------------>
	// <---  MATRICES  --->
	// <------------------>

	struct ANSEL_API mat4x4 {
		float m[4][4] = { 0 };

		void setIdentity() {
			m[0][0] = 1.f;
			m[1][1] = 1.f;
			m[2][2] = 1.f;
			m[3][3] = 1.f;
		}

		static mat4x4 makeTranslation(float x, float y, float z)
		{
			mat4x4 mat;

			mat.setIdentity();

			mat.m[3][0] = x;
			mat.m[3][1] = y;
			mat.m[3][2] = z;

			return mat;
		}

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
	};

	// 2D Vector Type Definitions
	typedef vec2<float>  vec2f;
	typedef vec2<double> vec2d;
	typedef vec2<int>    vec2i;
	typedef vec2<unsigned int> vec2u;

	// 3D Vector Type Definitions
	typedef vec3<float>  vec3f;
	typedef vec3<double> vec3d;
	typedef vec3<int>    vec3i;
	typedef vec3<unsigned int> vec3u;

	// 4D Vector Type Definitions
	typedef vec4<float>  vec4f;
	typedef vec4<double> vec4d;
	typedef vec4<int>    vec4i;
	typedef vec4<unsigned int> vec4u;
}