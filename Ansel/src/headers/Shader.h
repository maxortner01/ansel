#pragma once

#include "Def.h"

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

		void link() const;

		std::string readFile(const char *filename);
		GLenum getType(SHADER_TYPE shader);

	public:
		ANSEL_API  Shader(std::string vertex, std::string fragment);
		ANSEL_API ~Shader();

		void ANSEL_API makeShader(std::string filename, SHADER_TYPE type);
		void ANSEL_API bind();
		void ANSEL_API unbind();
	};
}