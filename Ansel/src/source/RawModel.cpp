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

		use_colors = true;
	}

	void RawModel::loadTextureCoordinates(std::vector<vec2f> coords, unsigned int texture) {
		vao->bind();

		Buffer::BUFFER_TYPE buffer = static_cast<Buffer::BUFFER_TYPE>(Buffer::TEX_COORDS0 + texture);

		if (!vao->bufferExists(Buffer::TEX_COORDS0 + texture))
			vao->genBuffer(buffer);

		vao->bindBufferData(coords, buffer);

		vao->unbind();

		use_textures = true;
	}
	
	void RawModel::loadTexture(std::string filename) {
		loadTexture( new Texture(filename) );
	}

	void RawModel::loadTexture(Texture* texture) {
		textures.push_back(texture);
	}

	void RawModel::bindTexures() {
		for (int i = 0; i < textures.size(); i++)
			textures.at(i)->bind(i);
	}

	void RawModel::unbindTexures() {
		for (Texture* tex : textures)
			tex->unBind();
	}

	void RawModel::loadNormals(std::vector<vec3f> normals) {
		vao->bind();

		if (!vao->bufferExists(Buffer::NORMAL))
			vao->genBuffer(Buffer::NORMAL);

		vao->bindBufferData(normals, Buffer::NORMAL);
		vao->unbind();

		use_normals = true;
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

	int RawModel::getTextureSize() const { return textures.size(); }

	VAO *RawModel::getVAO() const { return vao; }
	unsigned int RawModel::getVertexCount() const { return size; }
	unsigned int RawModel::getIndex() const { return index; }
	int RawModel::colorsOn() const { return use_colors; }
	int RawModel::normalsOn() const { return use_normals; }
	int RawModel::texturesOn() const { return use_textures; }

	RawModel::~RawModel() {
		vao->destroy();
	}
}