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
		rawModel->loadNormals(normals);

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
		std::vector<unsigned int> indices;

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
				reader >> word;
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
				reader >> word;
			}
			else if (strWord == "f") {
				// If all the normals and vertices are finished loading
				// make the size of the real normal list the same as the vertices
				if (realnormals.size() == 0)
					realnormals.resize(vertices.size());

				// Data struct for storing each face info
				typedef struct {
					unsigned int v, n, t;
				} Vertex;

				std::vector<Vertex> face;
				std::string line;

				while (1) {
					reader >> word;
					std::string faceValue(word);

					if (faceValue.length() <= 2) {
						break;
					}

					line += faceValue + " ";

					
				}

				auto faceStrings = splitString(' ', line);
				for (std::string f : faceStrings) {
					if (f.length() < 1)
						break;

					auto faces = splitString('/', f);

					Vertex vertex;
					int id = 0;
					for (std::string v : faces) {
						if (v == "")
							v = "0";

						/**/ if (id == 0) {
							vertex.v = std::stoi(v) - 1;
						} 
						else if (id == 1) {
							vertex.t = std::stoi(v) - 1;
						}
						else if (id == 2) {
							vertex.n = std::stoi(v) - 1;
						}

						id++;
					}
					face.push_back(vertex);
				}

				std::vector<Vertex> newFaces;

				/**/ if (face.size() == 3) {
					newFaces = face;
				}
				else if (face.size() == 4) {
					newFaces = {
						face.at(0),
						face.at(1),
						face.at(2),

						face.at(2),
						face.at(3),
						face.at(0)
					};
				}

				for (Vertex v : newFaces) {
					realnormals.at(v.v) += normals.at(v.n);
					indices.push_back(v.v);
				}
			}
			else {
				reader >> word;
			}

		}

		for (vec3f &normal : realnormals) {
			normal = normalize(normal);
		}

		return makeRawModel(vertices, indices, realnormals);
	}

	std::vector<std::string> Loader::splitString(const char delimiter, std::string str) {
		std::vector<std::string> f;
		str += delimiter;

		size_t pos = 0;
		std::string token;
		while ((pos = str.find(delimiter)) != std::string::npos) {
			token = str.substr(0, pos);

			f.push_back(token);

			str.erase(0, pos + 1);
		}

		return f;
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