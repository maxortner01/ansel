#include "../headers/Shader.h"

namespace Ansel
{
	Shader::Shader(std::string vertex, std::string fragment) {
		program = glCreateProgram();

		makeShader(vertex, VERTEX);
		makeShader(fragment, FRAGMENT);
		link();
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

	GLenum Shader::getType(SHADER_TYPE shader) {
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