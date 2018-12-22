#pragma once

#include "../../headers/util.h"

#include <vector>

namespace Ansel
{
	struct ModelContainer
	{
		std::vector<vec3f> vertices;
		std::vector<unsigned int> indices;
		std::vector<vec3f> normals;
		std::vector<vec4f> colors;
		std::vector<vec2f> tex;
	};
}