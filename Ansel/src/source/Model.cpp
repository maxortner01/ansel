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
	
	void RawModel::loadColors(std::vector<vec4f> colors) {
		// Bind the VAO
		vao->bind();
		// Generate a buffer at the Color location
		if (!vao->bufferExists(Buffer::COLOR))
			vao->genBuffer(Buffer::COLOR);
		// Bind the color information to the buffer
		vao->bindBufferData(colors, Buffer::COLOR);
		// Unbind VAO
		vao->unbind();
	}

	void RawModel::loadTransformations(std::vector<vec4f> transformation, std::vector<vec4f> rotation, std::vector<vec4f> scale) {
		// Bind the VAO
		vao->bind();

		Buffer::BUFFER_TYPE buffers[3] = {
			Buffer::TRANSFORMATION,
			Buffer::ROTATION,
			Buffer::SCALE
		};

		// Generate a buffer for transformations
		for (int i = 0; i < 3; i++) 
			if (!vao->bufferExists(buffers[i]))
				vao->genBuffer(buffers[i]);

		// Bind the transformation information to the buffer
		vao->bindBufferData(transformation, Buffer::TRANSFORMATION);
		vao->bindBufferData(rotation, Buffer::ROTATION);
		vao->bindBufferData(scale, Buffer::SCALE);

		// Unbind VAO
		vao->unbind();
	}
	
	VAO *RawModel::getVAO() const { return vao; }
	unsigned int RawModel::getVertexCount() const { return size; }
	unsigned int RawModel::getIndex() const { return index; }

	RawModel* Loader::makeRawModel(std::vector<vec2f> vertices) {
		VAO *vao = new VAO;

		// Create a VAO object and bind it
		vao->ID = createVAO();

		// Generate the Vertex Buffer Object
		vao->genBuffer(Buffer::VERTEX);

		// Bind the vertex information to the buffer
		vao->bindBufferData(vertices);

		// Finally, unbind the VAO
		vao->unbind();

		// Add the vao pointer to the vector of VAOS
		vaos.push_back(vao);

		// Create the final RawModel object with the VAO
		RawModel* rawModel = new RawModel(vao, (unsigned int)vertices.size(), nextModelIndex++);
		rawModels.push_back(rawModel);

		// Return the RawModel
		return rawModel;
	}

	RawModel* Loader::makeRawModel(std::vector<vec2f> vertices, std::vector<unsigned int> indices) {
		VAO *vao = new VAO;

		// Create a VAO object and bind it
		vao->ID = createVAO();

		// Generate both the Vertex and Index Buffer Objects
		vao->genBuffer(Buffer::VERTEX);
		vao->genBuffer(Buffer::INDICES);

		// Bind the vertex data
		vao->bindBufferData(vertices);

		// Bind the index data
		vao->bindBufferData(indices, NULL, Buffer::INDICES);
		
		// Unbind the vao
		vao->unbind();

		// Push VAO to the vector
		vaos.push_back(vao);

		// Create final RawModel object with the VAO
		RawModel* rawModel = new RawModel(vao, (unsigned int)indices.size(), nextModelIndex++);
		rawModels.push_back(rawModel);

		// Return RawModel
		return rawModel;
	}

	RawModel* Loader::makeRawModel(std::vector<vec3f> vertices, std::vector<unsigned int> indices) {
		VAO *vao = new VAO;

		// Calculate the normals
		std::vector<vec3f> normals;
		for (vec3f v : vertices) {

		}

		// Create a VAO object and bind it
		vao->ID = createVAO();

		// Generate both the Vertex and Index Buffer Objects
		vao->genBuffer(Buffer::VERTEX);
		vao->genBuffer(Buffer::INDICES);

		// Bind the vertex data
		vao->bindBufferData(vertices);

		// Bind the index data
		vao->bindBufferData(indices, NULL, Buffer::INDICES);

		// Unbind the vao
		vao->unbind();

		// Push VAO to the vector
		vaos.push_back(vao);

		// Create final RawModel object with the VAO
		RawModel* rawModel = new RawModel(vao, (unsigned int)indices.size(), nextModelIndex++);
		rawModels.push_back(rawModel);

		// Return RawModel
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

	Model* Loader::makeModel(RawModel* rawModel) {
		return new Model(rawModel);
	}

	/* <------------------------------------>*/
	/* <-------------- Model --------------->*/
	/* <------------------------------------>*/

	Model::Model() {
		_location = { 0, 0, 0, 1 };
		_rotation = { 0, 0, 0 };
		_scale    = { 1, 1, 1 };
	}

	Model::Model(RawModel* modelRef) {
		_ref = modelRef;

		_location = { 0, 0, 0, 1 };
		_rotation = { 0, 0, 0 };
		_scale = { 1, 1, 1 };
	}

	void Model::setLocation(vec4f location) {
		_location = location;
	}

	void Model::setLocation(float x, float y, float z, float w) {
		_location = { x, y, z, w };
	}

	void Model::addLocation(float x, float y, float z, float w) {
		_location = { _location.x + x, _location.y + y, _location.z + z, _location.w + w };
	}

	void Model::setLocation(vec2f location) {
		_location = { location.x, location.y, 1, 1 };
	}

	void Model::setLocation(float x, float y) {
		_location = { x, y, 1, 1 };
	}

	void Model::setRotation(vec3f rotation) {
		_rotation = rotation;
	}

	void Model::setRotation(float x, float y, float z) {
		setRotation({ x, y, z });
	}

	void Model::addRotation(float x, float y, float z) {
		_rotation = { _rotation.x + x, _rotation.y + y, _rotation.z + z };
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

	vec3f Model::getRotation() const {
		return _rotation;
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

	bool VAO::bufferExists(int type) {
		return (VBOS.at(type).count != 0);
	}

	void VAO::genBuffer(const Buffer::BUFFER_TYPE type) {
		Buffer buffer;
		glGenBuffers(1, &buffer.ID);
		buffer.type = type;
		VBOS.at(type) = buffer;
	}

	void VAO::bindBufferData(const std::vector<vec2f> data, const Buffer::BUFFER_TYPE type) {
		Buffer* buffer = &VBOS.at(type);
		buffer->bind();

		buffer->count = (unsigned int)data.size();
		buffer->data_size = 2;

		glBufferData(getType(type), data.size() * sizeof(vec2f), &data.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(type, buffer->data_size, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(type);

		buffer->unbind();
	}

	void VAO::bindBufferData(const std::vector<vec3f> data, const Buffer::BUFFER_TYPE type) {
		Buffer* buffer = &VBOS.at(type);
		buffer->bind();

		buffer->count = (unsigned int)data.size();
		buffer->data_size = 3;

		glBufferData(getType(type), data.size() * sizeof(vec3f), &data.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(type, buffer->data_size, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(type);

		buffer->unbind();
	}

	void VAO::bindBufferData(const std::vector<vec4f> data, Buffer::BUFFER_TYPE type) {
		Buffer* buffer = &VBOS.at(type);
		buffer->bind();

		buffer->count = (unsigned int)data.size();
		buffer->data_size = 4;

		if (type == Buffer::TRANSFORMATION || type == Buffer::ROTATION || type == Buffer::SCALE) {
			glBufferData(getType(type), data.size() * sizeof(vec4f), &data.front(), GL_DYNAMIC_DRAW);
			glVertexAttribPointer(type, buffer->data_size, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(type);

			glVertexAttribDivisor(type, 1);
		}
		else {
			glBufferData(getType(type), data.size() * sizeof(vec4f), &data.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(type, buffer->data_size, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(type);
		}

		buffer->unbind();
	}

	void VAO::bindBufferData(const std::vector<int> data, const unsigned int size, const Buffer::BUFFER_TYPE type) {
		Buffer* buffer = &VBOS.at(type);
		buffer->bind();

		buffer->count = (unsigned int)data.size();
		buffer->data_size = size;

		glBufferData(getType(type), data.size() * sizeof(int), &data.front(), GL_STATIC_DRAW);

		if (type != Buffer::INDICES)
			glVertexAttribPointer(type, buffer->data_size, GL_INT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(type);

		buffer->unbind();
	}

	void VAO::bindBufferData(const std::vector<unsigned int> data, const unsigned int size, const Buffer::BUFFER_TYPE type) {
		Buffer* buffer = &VBOS.at(type);
		buffer->bind();

		buffer->count = (unsigned int)data.size();
		buffer->data_size = size;

		glBufferData(getType(type), data.size() * sizeof(unsigned int), &data.front(), GL_STATIC_DRAW);

		if (type != Buffer::INDICES)
			glVertexAttribPointer(type, buffer->data_size, GL_UNSIGNED_INT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(type);

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