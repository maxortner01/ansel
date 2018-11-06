#pragma once

#include "Def.h"
#include "util.h"

#include <stdexcept>
#include <vector>
#include <GL/glew.h>

namespace Ansel
{
	// VBO object, each contains raw data for rendering
	struct Buffer
	{
		enum TYPE {
			VERTEX,
			COLOR,
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
	struct VAO
	{
		unsigned int nextAttribute = 0;
		unsigned int ID = 0;
		std::vector<Buffer> VBOS;

		VAO();

		static GLenum getType(Buffer::BUFFER_TYPE type);

		void genBuffer(const unsigned int attribute, const Buffer::BUFFER_TYPE type);
		void bindBufferData(const unsigned int attribute, const std::vector<vec2f> data, Buffer::BUFFER_TYPE type = Buffer::BUFFER_TYPE::VERTEX);
		void bindBufferData(const unsigned int attribute, const std::vector<vec4f> data, Buffer::BUFFER_TYPE type = Buffer::BUFFER_TYPE::VERTEX);
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

	class RawModel
	{
		VAO *vao;
		unsigned int size;
		unsigned int index;

	public:
		ANSEL_API RawModel();
		ANSEL_API RawModel(VAO *v, unsigned int s, unsigned int i);

		void ANSEL_API loadColor(std::vector<vec4f> colors);

		VAO ANSEL_API * getVAO() const;
		unsigned int ANSEL_API getVertexCount() const;
		unsigned int ANSEL_API getIndex() const;
	};

	class Model
	{
		vec4f _location;
		vec3f _scale;

		RawModel* _ref;

	public:
		ANSEL_API Model();
		ANSEL_API Model(RawModel* modelRef);

		void ANSEL_API setLocation(vec4f location);
		void ANSEL_API setScale(vec3f scale);


		vec4f	  ANSEL_API getLocation( ) const;
		vec3f	  ANSEL_API getScale( )    const;
		RawModel  ANSEL_API * getRawModel()  const;
	};

	class Loader
	{
		static std::vector<VAO*> vaos;
		static std::vector<RawModel*> rawModels;

		static int createVAO();

		static unsigned int nextModelIndex;

	public:

		static RawModel ANSEL_API * makeModel(std::vector<vec2f> vertices);
		static RawModel ANSEL_API * makeModel(std::vector<vec2f> vertices, std::vector<unsigned int> indices);
		static void  ANSEL_API destroy();

		static RawModel* getRawModel(const unsigned int index);
		static void destroyRawModel (const unsigned int index);
	};
}