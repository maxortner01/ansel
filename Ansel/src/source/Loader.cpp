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

	RawModel* Loader::makeRawModel(std::vector<vec3f> vertices) {
		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < vertices.size(); i++)
			indices.push_back(i);
		return makeRawModel(vertices, indices);
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

	RawModel* Loader::makeRawModel(std::vector<vec3f> vertices, std::vector<unsigned int> indices, std::vector<vec3f> normals) {
		VAO* vao = new VAO;

		// Create a VAO object and bind it
		vao->ID = createVAO();

		// Generate both the Vertex and Index Buffer Objects
		vao->genBuffer(Buffer::VERTEX);
		vao->genBuffer(Buffer::INDICES);
		vao->genBuffer(Buffer::NORMAL);

		// Bind the vertex data
		vao->bindBufferData(vertices);

		// Bind the index data
		vao->bindBufferData(indices, NULL, Buffer::INDICES);

		// Bind the normal data
		vao->bindBufferData(normals, Buffer::NORMAL);

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

	RawModel* Loader::makeRawModel(const char* filename) {
		
		std::ifstream reader;
		reader.open(filename);

		// If the reader is not open, return NULL or 
		// some sort of error object.
		if (!reader.is_open())
			return NULL;

		std::vector<vec3f> vertices, normals, realnormals;
		std::vector<unsigned int> indicies;

		// Read through the file
		unsigned int index = 0;
		unsigned int normal_index = 0;
		char word[50];
		reader >> word;
		while (reader.good()) {
			std::string strWord(word);
			
			/**/ if (strWord == "v") {
				vec3f loc;

				// X coordinate
				reader >> word;
				strWord = word;
				loc.x = std::stof(strWord);

				// Y coordinate
				reader >> word;
				strWord = word;
				loc.y = std::stof(strWord);

				// Z coordinate
				reader >> word;
				strWord = word;
				loc.z = std::stof(strWord);

				vertices.push_back(loc);
			}
			else if (strWord == "vn") {
				vec3f norm;

				// X coordinate
				reader >> word;
				strWord = word;
				norm.x = std::stof(strWord);

				// Y coordinate
				reader >> word;
				strWord = word;
				norm.y = std::stof(strWord);

				// Z coordinate
				reader >> word;
				strWord = word;
				norm.z = std::stof(strWord);

				normals.push_back(norm);
			}
			else if (strWord == "f") {
				std::vector<unsigned int> face;
				std::vector<vec3f> face_normals;

				for (int i = 0; i < 4; i++) {
					reader >> word;
					strWord = word;

					// Process the first two values of the face
					int id = 0;
					size_t pos = 0;
					std::string token, sIndex;
					while ((pos = strWord.find('/')) != std::string::npos) {
						token = strWord.substr(0, pos);

						if (id == 0) {
							sIndex = token;
						}

						strWord.erase(0, pos + 1);
						id++;
					}

					// What's left of strWord is the last value (the normal index)
					face_normals.push_back(normals.at(std::stoi(strWord) - 1));

					// Push the index to the indicies vector
					unsigned int index = std::stoi(sIndex) - 1;
					face.push_back(index);
				}

				indicies.push_back(face.at(0));
				realnormals.push_back(face_normals.at(0));
				indicies.push_back(face.at(1));
				realnormals.push_back(face_normals.at(1));
				indicies.push_back(face.at(2));
				realnormals.push_back(face_normals.at(2));

				indicies.push_back(face.at(2));
				realnormals.push_back(face_normals.at(2));
				indicies.push_back(face.at(3));
				realnormals.push_back(face_normals.at(3));
				indicies.push_back(face.at(0));
				realnormals.push_back(face_normals.at(0));
			}

			reader >> word;
		}

		normals.clear();
		normals.resize(vertices.size());
		for (int i = 0; i < indicies.size(); i++)
			normals.at(indicies.at(i)) = realnormals.at(i);

		return makeRawModel(vertices, indicies, normals);
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