#include "../headers/Model.h"

namespace Ansel
{
	/*! \brief Initializer for list of VAO pointers in the loader class */
	std::vector<VAO*> Loader::vaos;

	/*! \brief Initializer for the model class, empty so it can be uninitialized */
	Model::Model() {}

	/*! \brief Official initializer for the model class 
	 *  
	 * Initialized class with a pointer to a VAO struct. These structs are stored
	 * in the static vector in the Loader class.
	 */
	Model::Model(VAO *v, unsigned int s) {
		vao = v; size = s;
	}

	
	VAO *Model::getVAO() const { return vao; }
	unsigned int Model::getVertexCount() const { return size; }

	Model Loader::makeModel(std::vector<vec2f> vertices) {
		VAO *vao = new VAO;

		vao->ID = createVAO();
		vao->genBuffer(0, Buffer::VERTEX);
		vao->bindBufferData(0, vertices);
		vao->unbind();
		vaos.push_back(vao);

		return Model(vao, vertices.size());
	}

	Model Loader::makeModel(std::vector<vec2f> vertices, std::vector<unsigned int> indices) {
		VAO *vao = new VAO;

		vao->ID = createVAO();
		vao->unbind();

		vao->genBuffer(16, Buffer::INDICES);
		vao->genBuffer(0, Buffer::VERTEX);

		vao->bind();

		// Vertex data
		vao->bindBufferData(0, vertices);

		// Index data
		vao->bindBufferData(16, indices, NULL, Buffer::INDICES);
		
		vao->unbind();
		vaos.push_back(vao);

		return Model(vao, indices.size());
	}

	int Loader::createVAO() {
		unsigned int ID;

		glGenVertexArrays(1, &ID);
		glBindVertexArray(ID);

		return ID;
	}

	void Loader::destroy() {
		for (VAO *vao : vaos)
			vao->destroy();
	}

	void Buffer::bind()	{
		glBindBuffer(VAO::getType(type), ID);	
	}

	void Buffer::unbind()	{ glBindBuffer(GL_ARRAY_BUFFER, 0);  }

	VAO::VAO() {
		VBOS.resize(17);
	}

	GLenum VAO::getType(Buffer::BUFFER_TYPE type) {
		switch (type) {
			case Buffer::VERTEX:
				return GL_ARRAY_BUFFER;
				break;

			case Buffer::INDICES:
				return GL_ELEMENT_ARRAY_BUFFER;
				break;

			default:
				break;
		}
	}

	Buffer VAO::getBuffer(Buffer::BUFFER_TYPE type) {
		return VBOS.at(type);
	}

	bool VAO::bufferExists(Buffer::BUFFER_TYPE type) {
		return (VBOS.at(type).ID != 0);
	}

	void VAO::genBuffer(const unsigned int attribute, const Buffer::BUFFER_TYPE type) {
		Buffer buffer;
		glGenBuffers(1, &buffer.ID);
		buffer.type = type;
		VBOS.at(attribute) = buffer;
	}

	void VAO::bindBufferData(const unsigned int attribute, const std::vector<vec2f> data, const Buffer::BUFFER_TYPE type) {
		Buffer* buffer = &VBOS.at(attribute);
		buffer->bind();

		buffer->count = data.size();
		buffer->data_size = 2;

		glBufferData(getType(type), data.size() * sizeof(vec2f), &data.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(attribute, buffer->data_size, GL_FLOAT, GL_FALSE, 0, (void*)0);

		buffer->unbind();
	}

	void VAO::bindBufferData(const unsigned int attribute, const std::vector<int> data, const unsigned int size, const Buffer::BUFFER_TYPE type) {
		Buffer* buffer = &VBOS.at(attribute);
		buffer->bind();

		buffer->count = data.size();
		buffer->data_size = size;

		glBufferData(getType(type), data.size() * sizeof(int), &data.front(), GL_STATIC_DRAW);

		if (type != Buffer::INDICES)
			glVertexAttribPointer(attribute, buffer->data_size, GL_INT, GL_FALSE, 0, (void*)0);

		buffer->unbind();
	}

	void VAO::bindBufferData(const unsigned int attribute, const std::vector<unsigned int> data, const unsigned int size, const Buffer::BUFFER_TYPE type) {
		Buffer* buffer = &VBOS.at(attribute);
		buffer->bind();

		buffer->count = data.size();
		buffer->data_size = size;

		glBufferData(getType(type), data.size() * sizeof(unsigned int), &data.front(), GL_STATIC_DRAW);

		if (type != Buffer::INDICES)
			glVertexAttribPointer(attribute, buffer->data_size, GL_UNSIGNED_INT, GL_FALSE, 0, (void*)0);

		buffer->unbind();
	}

	void VAO::unbind() {
		glBindVertexArray(0);
	}

	void VAO::bind() {
		glBindVertexArray(ID);
	}

	void VAO::destroy() {
		for (Buffer VBO : VBOS)
			glDeleteBuffers(1, &VBO.ID);

		glDeleteVertexArrays(1, &ID);
	}
}