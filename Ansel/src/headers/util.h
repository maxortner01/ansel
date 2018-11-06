#pragma once

#include "Def.h"

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