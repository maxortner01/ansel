#include "../headers/Shader.h"

#include <iostream>
#include <fstream>
#include <GL/glew.h>

namespace Ansel
{
	Shader::Shader() {
		program = glCreateProgram();
	}

	Shader::Shader(std::string vertex, std::string fragment, bool l) {
		program = glCreateProgram();

		makeShader(vertex, VERTEX);
		makeShader(fragment, FRAGMENT);

		if (l) link();
	}

	Shader::~Shader() {
		glDeleteProgram(program);
	}

	void Shader::bind() {
		glUseProgram(program);
	}

	void Shader::unbind() {
		glUseProgram(0);
	}

	bool Shader::setUniform(float value, std::string name) {
		GLint location = glGetUniformLocation(program, name.c_str());

		if (location != -1)
			glUniform1f(location, value); return true;

		return false;
	}

	bool Shader::setUniform(int value, std::string name) {
		GLint location = glGetUniformLocation(program, name.c_str());

		if (location != -1)
			glUniform1i(location, value); return true;

		return false;
	}

	bool Shader::setUniform(vec2f value, std::string name) {
		GLint location = glGetUniformLocation(program, name.c_str());

		if (location != -1)
			glUniform2f(location, value.x, value.y); return true;

		return false;
	}

	bool Shader::setUniform(vec3f value, std::string name) {
		GLint location = glGetUniformLocation(program, name.c_str());

		if (location != -1)
			glUniform3f(location, value.x, value.y, value.z); return true;

		return false;
	}

	bool Shader::setUniform(vec4f value, std::string name) {
		GLint location = glGetUniformLocation(program, name.c_str());

		if (location != -1)
			glUniform4f(location, value.x, value.y, value.z, value.w); return true;

		return false;
	}

	bool Shader::setUniform(mat4x4 value, std::string name) {
		GLint location = glGetUniformLocation(program, name.c_str());

		if (location != -1)
			glUniformMatrix4fv(location, 1, GL_FALSE, *value.m); return true;

		return false;
	}

	bool Shader::setUniform(std::vector<int> value, std::string name, unsigned int size) {
		int location = glGetUniformLocation(program, name.c_str());

		if (location != -1)
			glUniform1iv(location, size, &value.front()); return true;

		return false;
	}

	bool Shader::setUniform(std::vector<vec4f> value, std::string name, unsigned int size) {
		const char* n = name.c_str();
		int location = glGetUniformLocation(program, n);

		std::vector<float> newvec;
		for (vec4f v : value) {
			newvec.push_back(v.x);
			newvec.push_back(v.y);
			newvec.push_back(v.z);
			newvec.push_back(v.w);
		}

		if (location != -1)
			glUniform4fv(location, size, newvec.data()); return true;

		return false;
	}

	bool Shader::setUniform(std::vector<vec3f> value, std::string name) {
		GLint location = glGetUniformLocation(program, name.c_str());

		std::vector<float> newvec;
		for (vec3f v : value) {
			newvec.push_back(v.x);
			newvec.push_back(v.y);
			newvec.push_back(v.z);
		}

		if (location != -1)
			glUniform3fv(location, INSTANCE_COUNT, newvec.data()); return true;

		return false;
	}

	void Shader::makeShader(std::string filename, SHADER_TYPE type) {
		shaders[type] = glCreateShader(getType(type));

		// Read shader source code
		std::string source   = readFile(filename.c_str());
		const char* c_source = source.c_str();

		GLint result = GL_FALSE;
		int logLength;

		glShaderSource(shaders[type], 1, &c_source, NULL);
		glCompileShader(shaders[type]);

		// Error testing
		glGetShaderiv(shaders[type], GL_COMPILE_STATUS, &result);
		glGetShaderiv(shaders[type], GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> shaderError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(shaders[type], logLength, NULL, &shaderError.front());
		std::cout << &shaderError.front() << std::endl;

		glAttachShader(program, shaders[type]);
	}

	void Shader::link() const {
		GLint result = GL_FALSE;
		int logLength;

		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &result);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> shaderError((logLength > 1) ? logLength : 1);
		glGetProgramInfoLog(program, logLength, NULL, &shaderError.front());
		std::cout << &shaderError.front() << std::endl;

		for (int i = 0; i < AMOUNT; i++)
			if (shaders[i] != 0)
				glDeleteShader(shaders[i]);

		std::cout << "done" << std::endl;
	}

	int Shader::getType(SHADER_TYPE shader) {
		switch (shader)
		{
		case Ansel::VERTEX:
			return GL_VERTEX_SHADER;
		case Ansel::FRAGMENT:
			return GL_FRAGMENT_SHADER;
		case Ansel::GEOMETRY:
			return GL_GEOMETRY_SHADER;
		case Ansel::AMOUNT:
			break;
		default:
			break;
		}
	}

	std::string Shader::readFile(const char* filename) {
		std::string content;
		std::ifstream fileStream(filename, std::ios::in);

		if (!fileStream.is_open()) {
			std::cout << "not good" << std::endl;
			return "not good.";
		}

		std::string line = "";
		while (!fileStream.eof()) {
			std::getline(fileStream, line);
			content.append(line + "\n");
		}

		fileStream.close();
		return content;
	}
}