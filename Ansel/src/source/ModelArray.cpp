#include "../headers/ModelArray.h"

namespace Ansel
{
	int ModelArray::pushRawModel(RawModel* rawModel) {
		const int index = rawModels.size();

		rawModels.push_back(rawModel);
		models.push_back({ });

		return index;
	}

	int ModelArray::pushModel(Model* model) {
		for (int i = 0; i < rawModels.size(); i++)
			if (rawModels.at(i) == model->getRawModel()) {
				models.at(i).push_back(model);
				return 1;
			}

		return 0;
	}

	RawModel* ModelArray::getRawModel(int loc) {
		return rawModels.at(loc);
	}

	void ModelArray::destroyRawModel(RawModel* rawModel) {
		int index = -1;
		for (int i = 0; i < rawModels.size(); i++)
			if (rawModels.at(i) == rawModel) {
				index = i;
				break;
			}

		if (index == -1)
			return;

		rawModels.erase(rawModels.begin() + index);
		models.erase(models.begin() + index);
	}

	void ModelArray::destroyModel(Model* model) {
		int index = -1;
		for (int i = 0; i < rawModels.size(); i++)
			if (rawModels.at(i) == model->getRawModel()) {
				index = i;
				break;
			}

		if (index == -1)
			return;

		int mindex = -1;
		for (int i = 0; i < models.at(index).size(); i++)
			if (models.at(index).at(i) == model) {
				mindex = i;
				break;
			}

		if (mindex == -1)
			return;

		models.at(index).erase(models.at(index).begin() + mindex);
	}
}