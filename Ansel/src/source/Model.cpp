#include "../headers/Model.h"

namespace Ansel
{
	/* <--------------------------------------->*/
	/* <-------------- Raw Model -------------->*/
	/* <--------------------------------------->*/
	/*! \brief Initializer for list of VAO pointers in the loader class */
	std::vector<VAO*> Loader::vaos;

	/*! \brief Initializer for list of pointers to RawModel classes */
	std::vector<RawModel*> Loader::rawModels;

	unsigned int Loader::nextModelIndex = 0;

	/*! \brief Initializer for the model class, empty so it can be uninitialized */
	RawModel::RawModel() {}

	/*! \brief Official initializer for the model class 
	 *  
	 * Initialized class with a pointer to a VAO struct. These structs are stored
	 * in the static vector in the Loader class.
	 */
	RawModel::RawModel(VAO *v, unsigned int s, unsigned int i) {
		vao = v; size = s; index = i;
	}
	
	void RawModel::loadColor(std::vector<vec4f> colors) {
		vao->genBuffer(Buffer::COLOR, Buffer::COLOR);
		vao->bindBufferData(Buffer::COLOR, colors, Buffer::COLOR);
	}
	
	VAO *RawModel::getVAO() const { return vao; }
	unsigned int RawModel::getVertexCount() const { return size; }
	unsigned int RawModel::getIndex() const { return index; }

	RawModel* Loader::makeModel(std::vector<vec2f> vertices) {
		VAO *vao = new VAO;

		vao->ID = createVAO();
		vao->genBuffer(0, Buffer::VERTEX);
		vao->bindBufferData(0, vertices);
		vao->unbind();
		vaos.push_back(vao);

		RawModel* rawModel = new RawModel(vao, (unsigned int)vertices.size(), nextModelIndex++);
		rawModels.push_back(rawModel);

		return rawModel;
	}

	RawModel* Loader::makeModel(std::vector<vec2f> vertices, std::vector<unsigned int> indices) {
		VAO *vao = new VAO;

		vao->ID = createVAO();
		vao->unbind();

		vao->genBuffer(0, Buffer::VERTEX);
		vao->genBuffer(16, Buffer::INDICES);
		vao->bind();

		// Vertex data
		vao->bindBufferData(0, vertices);

		// Index data
		vao->bindBufferData(16, indices, NULL, Buffer::INDICES);
		
		vao->unbind();
		vaos.push_back(vao);

		RawModel* rawModel = new RawModel(vao, (unsigned int)indices.size(), nextModelIndex++);
		rawModels.push_back(rawModel);

		return rawModel;
	}

	RawModel* Loader::getRawModel(const unsigned int index) {
		if (index > rawModels.size())
			throw std::invalid_argument("index not within range of created RawModels");

		return rawModels.at(index);
	}

	void Loader::destroyRawModel(const unsigned int index) {
		if (index > rawModels.size())
			throw std::invalid_argument("index not within range of created RawModels");;

		delete rawModels.at(index);
	}

	/* <------------------------------------>*/
	/* <-------------- Model --------------->*/
	/* <------------------------------------>*/

	Model::Model() {}

	Model::Model(RawModel* modelRef) {
		_ref = modelRef;
	}

	void Model::setLocation(vec4f location) {
		_location = location;
	}

	void Model::setScale(vec3f scale) {
		_scale = scale;
	}

	vec4f Model::getLocation() const {
		return _location;
	}

	vec3f Model::getScale() const {
		return _scale;
	}

	RawModel* Model::getRawModel() const {
		return _ref;
	}

	/* <-------------------------------------->*/
	/* <--------------- LOADER --------------->*/
	/* <-------------------------------------->*/
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
		VBOS.resize(Buffer::INDICES + 1);

		//for (int i = 0; i < VBOS.size(); i++)
		//	glGenBuffers(1, &VBOS.at(i).ID);
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
				return GL_ARRAY_BUFFER;
				break;
		}
	}

	Buffer VAO::getBuffer(Buffer::BUFFER_TYPE type) {
		return VBOS.at(type);
	}

	bool VAO::bufferExists(Buffer::BUFFER_TYPE type) {
		return (VBOS.at(type).count != 0);
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

		buffer->count = (unsigned int)data.size();
		buffer->data_size = 2;

		glBufferData(getType(type), data.size() * sizeof(vec2f), &data.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(attribute, buffer->data_size, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(attribute);

		buffer->unbind();
	}

	void VAO::bindBufferData(const unsigned int attribute, const std::vector<vec4f> data, Buffer::BUFFER_TYPE type) {
		Buffer* buffer = &VBOS.at(attribute);
		buffer->bind();

		buffer->count = (unsigned int)data.size();
		buffer->data_size = 4;

		glBufferData(getType(type), data.size() * sizeof(vec4f), &data.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(attribute, buffer->data_size, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(attribute);

		buffer->unbind();
	}

	void VAO::bindBufferData(const unsigned int attribute, const std::vector<int> data, const unsigned int size, const Buffer::BUFFER_TYPE type) {
		Buffer* buffer = &VBOS.at(attribute);
		buffer->bind();

		buffer->count = (unsigned int)data.size();
		buffer->data_size = size;

		glBufferData(getType(type), data.size() * sizeof(int), &data.front(), GL_STATIC_DRAW);

		if (type != Buffer::INDICES)
			glVertexAttribPointer(attribute, buffer->data_size, GL_INT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(attribute);

		buffer->unbind();
	}

	void VAO::bindBufferData(const unsigned int attribute, const std::vector<unsigned int> data, const unsigned int size, const Buffer::BUFFER_TYPE type) {
		Buffer* buffer = &VBOS.at(attribute);
		buffer->bind();

		buffer->count = (unsigned int)data.size();
		buffer->data_size = size;

		glBufferData(getType(type), data.size() * sizeof(unsigned int), &data.front(), GL_STATIC_DRAW);

		if (type != Buffer::INDICES)
			glVertexAttribPointer(attribute, buffer->data_size, GL_UNSIGNED_INT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(attribute);

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