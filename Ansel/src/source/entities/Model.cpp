#include "../../headers/entities/Model.h"

#include <fstream>

#include <stdexcept>
#include <algorithm>

#include <GL/glew.h>

namespace Ansel
{
	/* <------------------------------------>*/
	/* <-------------- Model --------------->*/
	/* <------------------------------------>*/

	Model::Model() {
		_location = { 0, 0, 0, 1 };
		_color    = { 2, 2, 2, 2 };
		_rotation = { 0, 0, 0 };
		_scale    = { 1, 1, 1 };

		__derivative = 0;
	}

	Model::Model(RawModel* modelRef) {
		_ref = modelRef;

		_location = { 0, 0, 0, 1 };
		_color    = { 2, 2, 2, 2 };
		_rotation = { 0, 0, 0 };
		_scale    = { 1, 1, 1 };

		__derivative = 0;
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

	void Model::setColor(vec4f color) {
		_color = color;
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

	/* <------------------------------------>*/
	/* <--------------  VBO  --------------->*/
	/* <------------------------------------>*/

	void Buffer::bind()	{
		glBindBuffer(VAO::getType(type), ID);	
	}

	void Buffer::unbind() { 
		glBindBuffer(GL_ARRAY_BUFFER, 0);  
	}

	/* <------------------------------------>*/
	/* <--------------  VAO  --------------->*/
	/* <------------------------------------>*/

	VAO::VAO() {
		VBOS.resize(Buffer::INDICES + 1);
	}

	int VAO::getType(Buffer::BUFFER_TYPE type) {
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

	void VAO::bindBufferData(const std::vector<vec2f> data, const Buffer::BUFFER_TYPE type, bool dynamic) {
		Buffer* buffer = &VBOS.at(type);
		buffer->bind();

		buffer->count = (unsigned int)data.size();
		buffer->data_size = 2;

		glBufferData(getType(type), data.size() * sizeof(vec2f), &data.front(), (dynamic)?GL_DYNAMIC_DRAW:GL_STATIC_DRAW);
		glVertexAttribPointer(type, buffer->data_size, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(type);

		buffer->unbind();
	}

	void VAO::bindBufferData(const std::vector<vec3f> data, const Buffer::BUFFER_TYPE type, bool dynamic) {
		Buffer* buffer = &VBOS.at(type);
		buffer->bind();

		buffer->count = (unsigned int)data.size();
		buffer->data_size = 3;

		glBufferData(getType(type), data.size() * sizeof(vec3f), &data.front(), (dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glVertexAttribPointer(type, buffer->data_size, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(type);

		buffer->unbind();
	}

	void VAO::bindBufferData(const std::vector<vec4f> data, Buffer::BUFFER_TYPE type, bool divisor, bool dynamic) {
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
			glBufferData(getType(type), data.size() * sizeof(vec4f), &data.front(), (dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
			glVertexAttribPointer(type, buffer->data_size, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(type);

			if (divisor) { glVertexAttribDivisor(type, 1); }
		}

		buffer->unbind();
	}

	void VAO::bindBufferData(const std::vector<int> data, const unsigned int size, const Buffer::BUFFER_TYPE type, bool dynamic) {
		Buffer* buffer = &VBOS.at(type);
		buffer->bind();

		buffer->count = (unsigned int)data.size();
		buffer->data_size = size;

		glBufferData(getType(type), data.size() * sizeof(int), &data.front(), (dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

		if (type != Buffer::INDICES)
			glVertexAttribPointer(type, buffer->data_size, GL_INT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(type);

		buffer->unbind();
	}

	void VAO::bindBufferData(const std::vector<unsigned int> data, const unsigned int size, const Buffer::BUFFER_TYPE type, bool dynamic) {
		Buffer* buffer = &VBOS.at(type);
		buffer->bind();

		buffer->count = (unsigned int)data.size();
		buffer->data_size = size;

		glBufferData(getType(type), data.size() * sizeof(unsigned int), &data.front(), (dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

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