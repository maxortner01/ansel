#include "../headers/Model.h"

namespace Ansel
{
	/*! \brief Initializer for list of VAO pointers in the loader class */
	std::vector<VAO*> Loader::vaos;

	/*! \brief Initializer for list of pointers to RawModel classes */
	std::vector<RawModel*> Loader::rawModels;
	unsigned int Loader::nextModelIndex = 0;


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

		/* Calculate the normals
		std::vector<vec4f> normals;
		for (int i = 0; i < vertices.size() / 3; i++) {
			vec3f origin = vertices.at(i * 3);
			vec3f a = vertices.at(i * 3 + 1);
			vec3f b = vertices.at(i * 3 + 2);

			a = { a.x - origin.x, a.y - origin.y, a.z - origin.z };
			b = { b.x - origin.x, b.y - origin.y, b.z - origin.z };

			vec4f n = {
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x,
				1
			};

			float l = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
			n = { n.x / l, n.y / l, n.z / l, 1 };

			for (int j = 0; j < 3; j++)
				normals.push_back(n);
		} */

		// Create a VAO object and bind it
		vao->ID = createVAO();

		// Generate both the Vertex and Index Buffer Objects
		vao->genBuffer(Buffer::VERTEX);
		vao->genBuffer(Buffer::INDICES);

		// Bind the vertex data
		vao->bindBufferData(vertices);

		// Bind the index data
		vao->bindBufferData(indices, NULL, Buffer::INDICES);

		// Bind normal data
		//vao->bindBufferData(normals, Buffer::NORMAL);

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
}