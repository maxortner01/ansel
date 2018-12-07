#include "../../headers/entities/Model.h"

namespace Ansel
{
	unsigned int RawModel::count = 0;

	/*! \brief Initializer for the model class, empty so it can be uninitialized */
	RawModel::RawModel() {}

	/*! \brief Official initializer for the model class
	 *
	 * Initialized class with a pointer to a VAO struct. These structs are stored
	 * in the static vector in the Loader class.
	 */
	RawModel::RawModel(VAO *v, unsigned int s, unsigned int i) {
		vao = v; size = s; index = i;
		ID = count++;
	}

	unsigned int RawModel::getID() {
		return ID;
	}

	void RawModel::loadColors(std::vector<vec4f> colors) {
		// Bind the VAO
		vao->bind();
		// Generate a buffer at the Color location
		if (!vao->bufferExists(Buffer::COLOR))
			vao->genBuffer(Buffer::COLOR);

		bool multi_colors = !(colors.size() == size);

		// Bind the color information to the buffer
		vao->bindBufferData(colors, Buffer::COLOR, multi_colors, true);
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

		//use_textures = true;
	}
	
	int RawModel::loadTexture(std::string filename) {
		return loadTexture( new Texture(filename) );
	}

	int RawModel::loadTexture(Texture* texture) {

		use_textures = true;
		textures.push_back(texture);
		return getTextureSize() - 1;
	}

	void RawModel::loadMaterial(Material* material) {
		_material = material;
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

	Texture* RawModel::getTexture(int index) const {
		return textures.at(index);
	}

	Material* RawModel::getMaterial() const {
		return _material;
	}

	VAO *RawModel::getVAO() const { return vao; }
	unsigned int RawModel::getVertexCount() const { return size; }
	unsigned int RawModel::getIndex() const { return index; }

	int RawModel::colorsOn() const { return use_colors; }
	int RawModel::normalsOn() const { return use_normals; }
	int RawModel::texturesOn() const { return use_textures; }

	void RawModel::setColorsOn(bool isOn) {
		use_colors = isOn;
	}

	void RawModel::setNormalsOn(bool isOn) {
		use_normals = isOn;
	}

	void RawModel::setTexturesOn(bool isOn) {
		use_textures = isOn;
	}

	RawModel::~RawModel() {
		vao->destroy();
	}
}