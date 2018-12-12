#include "../../headers/entities/Model.h"

#include <fstream>

#include <stdexcept>
#include <algorithm>

#include <GL/glew.h>
#include <fbxsdk.h>
#include <iostream>

#include <ECS.h>

using namespace ECS;

namespace Ansel
{

	/*! \brief Initializer for list of VAO pointers in the loader class */
	std::vector<VAO*> Loader::vaos;

	std::vector<std::string> Loader::names;
	std::vector<std::string> Loader::fontNames;

	/*! \brief Initializer for list of pointers to RawModel classes */
	std::vector<RawModel*> Loader::rawModels;
	std::vector<Model*> Loader::models;
	std::vector<Font*> Loader::fonts;

	unsigned int Loader::nextModelIndex = 0;

	RawModel* Loader::readOBJ(const char* filename, const char* storage_name, bool smooth_shading) {
		std::ifstream reader;
		reader.open(filename);

		// If the reader is not open, return NULL or 
		// some sort of error object.
		if (!reader.is_open())
			return NULL;

		std::vector<vec3f> vertices, normals, realnormals, realvertices;
		std::vector<vec2f> tex_coords, tex;
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
			else if (strWord == "vt") {
				vec2f t;

				reader >> word;
				strWord = word;
				t.x = std::stof(word);

				reader >> word;
				strWord = word;
				t.y = std::stof(word);

				tex.push_back(t);
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
				if (realnormals.size() == 0 && smooth_shading)
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

				if (smooth_shading) {
					for (Vertex v : newFaces) {
						realnormals.at(v.v) += normals.at(v.n);
						indices.push_back(v.v);
					}
				}
				else {
					for (Vertex v : newFaces) {
						realvertices.push_back(vertices.at(v.v));
						//realnormals.push_back(normals.at(v.n));

						if (tex.size() > 0) {
							vec2f t = tex.at(v.t);
							t = { abs(t.x), abs(t.y) };

							tex_coords.push_back(t);
						}

						indices.push_back(index++);
					}
				}
			}
			else {
				reader >> word;
			}

		}

		for (vec3f &normal : realnormals) {
			normal = normalize(normal);
		}

		RawModel* r;

		if (smooth_shading)
			r = makeRawModel(vertices, indices, realnormals, storage_name);
		else
			r = makeRawModel(realvertices, indices, storage_name);

		if (tex_coords.size() > 0)
			r->loadTextureCoordinates(tex_coords);

		return r;
	}

	void addMesh(FbxNode* node, std::vector<FbxMesh*> *meshes) {
		FbxNodeAttribute::EType nodeType = node->GetNodeAttribute()->GetAttributeType();

		if (nodeType == FbxNodeAttribute::eMesh) {
			meshes->push_back((FbxMesh*)node->GetNodeAttribute());
		}

		for (int i = 0; i < node->GetChildCount(); i++) {
			addMesh(node->GetChild(i), meshes);
		}
	}

	RawModel* Loader::readFBX(const char* filename, const char* storage_name, bool smooth_shading) {
		FbxManager*  manager  = FbxManager::Create();
		FbxIOSettings*   ios  = FbxIOSettings::Create(manager, IOSROOT);
		FbxImporter* importer = FbxImporter::Create(manager, "");

		if (!importer->Initialize(filename, -1, manager->GetIOSettings())) {
			std::cout << "FBXImporter failed to initialize!\n";
			std::cout << "Error: " << importer->GetStatus().GetErrorString() << "\n";
			return nullptr;
		}

		std::vector<FbxMesh*> meshes;

		FbxScene* scene = FbxScene::Create(manager, "scene");
		importer->Import(scene);
		importer->Destroy();

		FbxNode* rootNode = scene->GetRootNode();
		
		for (int i = 0; i < rootNode->GetChildCount(); i++) {
			addMesh(rootNode->GetChild(i), &meshes);
		}
		
		std::vector<vec3f> vertices, normals;
		std::vector<vec2f> tex;
		std::vector<unsigned int> indices;

		for (int i = 0; i < meshes.size(); ++i) {
			FbxMesh* mesh = meshes.at(i);

			if (mesh == nullptr) continue;

			int polyCount = mesh->GetPolygonCount();
			FbxVector4* controlPoints = mesh->GetControlPoints();
			int controlPointCount = mesh->GetControlPointsCount();

			int vertexID = 0;

			for (int polygon = 0; polygon < polyCount; polygon++) {
				int polyVertCount = mesh->GetPolygonSize(polygon);
				
				for (int polyVert = 0; polyVert < polyVertCount; polyVert++) {
					vec3f ansVert, ansNorm;
					vec2f ansUV;

					int index = mesh->GetPolygonVertex(polygon, polyVert);

					ansVert.x = (float)controlPoints[index].mData[0];
					ansVert.y = (float)controlPoints[index].mData[1];
					ansVert.z = (float)controlPoints[index].mData[2];

					size_t size = vertices.size();
					size_t i;

					for (i = 0; i < size; i++) {
						if (ansVert == vertices.at(i)) {
							break;
						}
					}

					if (i == size)
					{
						vertices.push_back(ansVert);

						indices.push_back(i);
						++vertexID;
						continue;
					}

					int uvElementCount = mesh->GetElementUVCount();
					for (int uvElement = 0; uvElement < uvElementCount; uvElement++) {
						FbxGeometryElementUV* geoUV = mesh->GetElementUV(uvElement);
						
						FbxLayerElement::EMappingMode mapMode = geoUV->GetMappingMode();
						FbxLayerElement::EReferenceMode refMode = geoUV->GetReferenceMode();

						int directIndex = -1;

						/**/ if (FbxGeometryElement::eByControlPoint == mapMode) {
							if (FbxGeometryElement::eDirect == refMode) {
								directIndex = index;
							}
							else if (FbxGeometryElement::eIndexToDirect == refMode) {
								directIndex = geoUV->GetIndexArray().GetAt(index);
							}
						}
						else if (FbxGeometryElement::eByPolygonVertex == mapMode) {
							if (FbxGeometryElement::eDirect == refMode || FbxGeometryElement::eIndexToDirect == refMode) {
								directIndex = mesh->GetTextureUVIndex(polygon, polyVert);
							}
						}

						if (directIndex != -1) {
							FbxVector2 uv = geoUV->GetDirectArray().GetAt(directIndex);

							ansUV.x = (float)uv.mData[0];
							ansUV.y = (float)uv.mData[1];
						}
					}

					int normElementCount = mesh->GetElementNormalCount();

					for (int normElement = 0; normElement < normElementCount; normElement++) {
						FbxGeometryElementNormal* geoElementNormal = mesh->GetElementNormal(normElement);

						FbxLayerElement::EMappingMode mapMode = geoElementNormal->GetMappingMode();
						FbxLayerElement::EReferenceMode refMode = geoElementNormal->GetReferenceMode();
						
						int directIndex = -1;

						if (FbxGeometryElement::eByPolygonVertex == mapMode) {
							/**/ if (FbxGeometryElement::eDirect == refMode) {
								directIndex = vertexID;
							}
							else if (FbxGeometryElement::eIndexToDirect == refMode) {
								directIndex = geoElementNormal->GetIndexArray().GetAt(vertexID);
							}
						}

						if (directIndex != -1) {
							FbxVector4 norm = geoElementNormal->GetDirectArray().GetAt(directIndex);

							ansNorm.x = (float)norm.mData[0];
							ansNorm.y = (float)norm.mData[1];
							ansNorm.z = (float)norm.mData[2];
						}
					}
				}

				indices.push_back(i);
				++vertexID;
			}
		}

		// Read the stuff

		manager->Destroy();

		return Loader::makeRawModel(vertices, indices, storage_name);
	}

	ECS::Entity* Loader::makeEntity(std::string name) {
		return ECS::EntityManager::makeEntity(name.c_str());
	}

	ECS::Entity* Loader::getEntity(std::string name) {
		return ECS::EntityManager::getEntity(name.c_str());
	}

	std::vector<ECS::EntityInstance> Loader::getEntities() {
		return ECS::EntityManager::getEntities();
	}

	RawModel* Loader::getRawModel(const char* storage_name) {
		if (storage_name == "") return nullptr;

		int index = -1;
		for (int i = 0; i < names.size(); i++)
			if (names.at(i) == storage_name) {
				index = i; break;
			}

		if (index == -1) return nullptr;
		return rawModels.at(index);
	}

	RawModel* Loader::makeRawModel(std::vector<vec2f> vertices, const char* storage_name) {
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
		names.push_back(storage_name);

		// Return the RawModel
		return rawModel;
	}

	RawModel* Loader::makeRawModel(std::vector<vec3f> vertices, const char* storage_name) {
		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < vertices.size(); i++)
			indices.push_back(i);
		return makeRawModel(vertices, indices, storage_name);
	}

	RawModel* Loader::makeRawModel(std::vector<vec2f> vertices, std::vector<unsigned int> indices, const char* storage_name) {
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
		names.push_back(storage_name);

		// Return RawModel
		return rawModel;
	}

	RawModel* Loader::makeRawModel(std::vector<vec3f> vertices, std::vector<unsigned int> indices, const char* storage_name) {
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
		names.push_back(storage_name);

		// Return RawModel
		return rawModel;
	}

	RawModel* Loader::makeRawModel(std::vector<vec3f> vertices, std::vector<unsigned int> indices, std::vector<vec3f> normals, const char* storage_name) {
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
		names.push_back(storage_name);

		// Return RawModel
		return rawModel;
	}

	RawModel* Loader::makeRawModel(const char* filename, const char* storage_name, bool smooth_shading) {
		std::string str(filename); 
		std::string type = str.substr(str.length() - 4, str.length() - 1);
		std::transform(type.begin(), type.end(), type.begin(), ::tolower);

		RawModel* r = nullptr;
		/**/ if (type == ".obj") { r = readOBJ(filename, storage_name, smooth_shading); }
		else if (type == ".fbx") { r = readFBX(filename, storage_name, smooth_shading); }

		return r;
	}

	Font* Loader::makeFont(const char* filename, const char* name, unsigned int size) {
		Font* font = new Font(filename, size);
		
		fontNames.push_back(name);
		fonts.push_back(font);

		return font;
	}

	Font* Loader::getFont(const char* name) {
		if (name == "") return nullptr;

		int index = -1;
		for (int i = 0; i < fontNames.size(); i++)
			if (fontNames.at(i) == name) {
				index = i; break;
			}

		if (index == -1) return nullptr;
		return fonts.at(index);
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
		Model* m = new Model(rawModel);

		models.push_back(m);

		return m;
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


	RawModel* Loader::makeSquare() {
		std::vector<vec3f> square = {
			{ -1.f, -1.f, 0.f },
			{  1.f, -1.f, 0.f },
			{ -1.f,  1.f, 0.f },
			{  1.f,  1.f, 0.f }
		};

		std::vector<vec3f> normals = {
			{ 0, 0, 1 },
			{ 0, 0, 1 },
			{ 0, 0, 1 },
			{ 0, 0, 1 }
		};

		std::vector<vec4f> colors = {
			{ 1.f, 0.f, 0.f, 1.f },
			{ 1.f, 0.f, 0.f, 1.f },
			{ 1.f, 0.f, 0.f, 1.f },
			{ 1.f, 0.f, 0.f, 1.f },
			{ 1.f, 0.f, 0.f, 1.f },
			{ 1.f, 0.f, 0.f, 1.f }
		};

		std::vector<unsigned int> indices =	{
			1, 2, 0,
			1, 3, 2
		};

		std::vector<vec2f> tex = {
			{ 0, 0 },
			{ 1, 0 },
			{ 0, 1 },
			{ 1, 1 }
		};

		RawModel* model = makeRawModel(square, indices);
		model->loadNormals(normals);
		model->loadTextureCoordinates(tex);
		model->loadColors(colors);

		return model;
	}
}