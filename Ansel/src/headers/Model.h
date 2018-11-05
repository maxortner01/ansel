#pragma once

#include "Def.h"
#include "util.h"

#include <vector>
#include <GL/glew.h>

namespace Ansel
{
	// VBO object, each contains raw data for rendering
	struct ANSEL_API Buffer
	{
		enum TYPE {
			VERTEX,
			INDICES = 16
		} typedef BUFFER_TYPE;

		GLuint ID = 0;
		unsigned int data_size = 0;
		unsigned int count = 0;
		BUFFER_TYPE type;

		void bind();
		void unbind();
	};

	// Vertex Array Object, each one holds vertex data
	struct ANSEL_API VAO
	{
		unsigned int nextAttribute = 0;
		unsigned int ID = 0;
		std::vector<Buffer> VBOS;

		VAO();

		static GLenum getType(Buffer::BUFFER_TYPE type);

		void genBuffer(const unsigned int attribute, const Buffer::BUFFER_TYPE type);
		void bindBufferData(const unsigned int attribute, const std::vector<vec2f> data, Buffer::BUFFER_TYPE type = Buffer::BUFFER_TYPE::VERTEX);
		void bindBufferData(const unsigned int attribute, const std::vector<int> data, 
			const unsigned int size = 2, const Buffer::BUFFER_TYPE type = Buffer::BUFFER_TYPE::VERTEX);
		void bindBufferData(const unsigned int attribute, const std::vector<unsigned int> data,
			const unsigned int size = 2, const Buffer::BUFFER_TYPE type = Buffer::BUFFER_TYPE::VERTEX);

		Buffer getBuffer(Buffer::BUFFER_TYPE type);
		bool   bufferExists(Buffer::BUFFER_TYPE type);

		void bind();
		void unbind();

		void destroy();
	};

	class Model
	{
		VAO *vao;
		unsigned int size;

	public:
		ANSEL_API Model();
		ANSEL_API Model(VAO *v, unsigned int s);

		VAO ANSEL_API *getVAO() const;
		unsigned int ANSEL_API getVertexCount() const;
	};

	class Loader
	{

		static std::vector<VAO*> vaos;

		static int createVAO();

	public:

		static Model ANSEL_API makeModel(std::vector<vec2f> vertices);
		static Model ANSEL_API makeModel(std::vector<vec2f> vertices, std::vector<unsigned int> indices);
		static void  ANSEL_API destroy();

	};
}