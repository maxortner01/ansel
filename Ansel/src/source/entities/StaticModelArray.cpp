#include "../../headers/entities/StaticModelArray.h"

namespace Ansel
{
	StaticModelArray::StaticModelArray() {}

	StaticModelArray::StaticModelArray(std::vector<Model*> m) {
		models = m;
	}

	void StaticModelArray::addModel(Model* model) {
		models.push_back(model);
	}

	void StaticModelArray::removeModel(Model* model) {
		for (int i = 0; i < models.size(); i++) {
			if (models.at(i) == model) {
				models.erase(models.begin() + i); 
				break;
			}
		}
	}

	std::vector<Model*> StaticModelArray::getModels() const {
		return models;
	}

	std::vector<vec4f> StaticModelArray::getTransformation(TRANSFORMATION transformation) const {
		switch (transformation)
		{
			case LOCATIONS:
				return locations;
			
			case ROTATION:
				return rotations;
			
			case SCALES:
				return scales;

			default:
				break;
		}
	}

	void StaticModelArray::bake() {
		for (Model* m : models) {
			vec4f location = m->getLocation();
			locations.push_back(location);

			vec3f rotation = m->getRotation();
			rotations.push_back({ rotation.x, rotation.y, rotation.z, 1 });

			vec3f scale = m->getScale();
			scales.push_back({ scale.x, scale.y, scale.z, 1 });
		}

		Model* m = models.at(0);
		models.clear();

		models.push_back(m);
	}
}