#pragma once

#include "Def.h"
#include "util.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <GL/glew.h>

namespace Ansel
{
	enum SHADER_TYPE {
		VERTEX,
		FRAGMENT,
		GEOMETRY,
		AMOUNT
	} typedef SHADER_TYPE;

	class Shader
	{
		unsigned int shaders[SHADER_TYPE::AMOUNT];
		unsigned int program;

		std::string readFile(const char *filename);
		GLenum getType(SHADER_TYPE shader);

	public:
		ANSEL_API  Shader();
		ANSEL_API  Shader(std::string vertex, std::string fragment);
		ANSEL_API ~Shader();

		bool ANSEL_API setUniform(float value, std::string name);
		bool ANSEL_API setUniform(vec2f value, std::string name);
		bool ANSEL_API setUniform(vec4f value, std::string name);
		bool ANSEL_API setUniform(mat4x4 value, std::string name);
		bool ANSEL_API setUniform(std::vector<vec4f> value, std::string name, unsigned int size);
		bool ANSEL_API setUniform(std::vector<vec3f> value, std::string name);

		void ANSEL_API makeShader(std::string filename, SHADER_TYPE type);
		void link() const;

		void ANSEL_API bind();
		void ANSEL_API unbind();
	};
}