#include "../headers/Model.h"

namespace Ansel
{
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
}