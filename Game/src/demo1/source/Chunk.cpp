#include "../headers/Chunk.h"

using namespace Ansel;

namespace Demo1 {
	unsigned int Chunk::count = 0;
	unsigned int Chunk::width_amount = 100;
	float Chunk::size = 2.f;

	float Chunk::getTerrain(vec2f location) {
		float noise = SimplexNoise::noise(location.x / 750.f, location.y / 750.f);
		
		noise = -abs(noise) * 2 + 1;

		noise *= (SimplexNoise::noise(location.x / 350.f, location.y / 350.f) + 1) / 2.f;

		return noise * 18.f;
	}

	void Chunk::genChunk(ModelContainer* container, vec2i chunk_loc) {
		std::vector<vec4f> colors;
		std::vector<unsigned int> indices;
		std::vector<vec3f> vertices, normals;
		std::vector<vec2f> tex;

		std::vector<vec2f> tex_samples = {
			{ 0, 1 },
			{ 0, 0 },
			{ 1, 0 },
			{ 1, 1 },
			{ 0, 1 },
			{ 1, 0 }
		};

		for (int z = 0; z <= width_amount; z++) {
			for (int x = 0; x <= width_amount; x++) {
				const float X = (float)chunk_loc.x * size * width_amount + x * size;
				const float Z = (float)chunk_loc.y * size * width_amount + z * size;

				if (z != width_amount && x != width_amount) {
					indices.push_back(z * (width_amount + 1) + x);
					indices.push_back((z + 1) * (width_amount + 1) + x + 1);
					indices.push_back(z * (width_amount + 1) + x + 1);

					indices.push_back((z + 1) * (width_amount + 1) + x);
					indices.push_back((z + 1) * (width_amount + 1) + x + 1);
					indices.push_back(z * (width_amount + 1) + x);
				}

				vec3f vertex = { X, getTerrain({ X, Z }), Z };

				vertices.push_back(vertex);
				colors.push_back({ 1, 1, 1, 1 });
				tex.push_back({ (float)(z % 2),  (float)(x % 2) });
			}
		}

		normals.resize(vertices.size());
		for (int i = 0; i < indices.size(); i += 3) {
			vec3f normal;
			vec3f v1 = vertices.at(indices.at(i));
			vec3f v2 = vertices.at(indices.at(i + 1));
			vec3f v3 = vertices.at(indices.at(i + 2));

			vec3f e1 = v2 - v1;
			vec3f e2 = v3 - v1;

			normal = cross(e1, e2);
			normals.at(indices.at(i)) += normal;
			normals.at(indices.at(i + 1)) += normal;
			normals.at(indices.at(i + 2)) += normal;
		}

		for (int i = 0; i < normals.size(); i++)
			normals.at(i) = normalize(normals.at(i));

		container->colors = colors;
		container->vertices = vertices;
		container->normals = normals;
		container->indices = indices;
		container->tex = tex;

		//RawModel* temp = Loader::makeRawModel(vertices, indices, normals, ("chunk" + std::to_string(count++)).c_str(), false);
		//*rawModel = *temp;
		//delete temp;
		//
		//rawModel->loadColors(colors);
		//rawModel->loadTextureCoordinates(tex);
		//
		//Material* material = new Material;
		//material->loadTexture(Material::ALBEDO, new Texture("assets/textures/sands/sand6.png"));
		//
		//rawModel->loadMaterial(material);
	}

	Chunk::Chunk(vec2i chunk_loc) {
		ID = count;

		realLocation = {
			(float)chunk_loc.x * size * width_amount,
			(float)chunk_loc.y * size * width_amount
		};

		ModelContainer container;
		genChunk(&container, chunk_loc);

		_rawModel = Loader::makeRawModel(container.vertices, container.indices, container.normals, std::to_string(ID).c_str());
		_rawModel->loadColors(container.colors);
		_rawModel->loadTextureCoordinates(container.tex);

		Material* material = new Material;
		material->loadTexture(Material::ALBEDO, new Texture("assets/textures/sands/sand6.png"));
		
		_rawModel->loadMaterial(material);

		Model* model = Loader::makeModel(_rawModel);
		
		addComponent(model, "model");
	}

	Chunk::Chunk(RawModel* rawModel) {
		ID = count++;

		Model* model = Loader::makeModel(rawModel);
		addComponent(model, "model");
	}

	Chunk::~Chunk() {
		delete _rawModel;
	}
}