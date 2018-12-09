#include "../../headers/rendering/Text.h"

namespace Ansel
{
	std::vector<vec3f> Text::vertices = {
		{ -1.f, -1.f,  0.f },
		{ -1.f,  1.f,  0.f },
		{  1.f,  1.f,  0.f },
		{  1.f, -1.f,  0.f }
	};

	RawModel* Text::rawModel = nullptr;

	bool Text::initialized = false;

	Text::Text() {
		if (!initialized) {
			const std::vector<vec2f> tex = {
				{ 0, 1 },
				{ 0, 0 },
				{ 1, 0 },
				{ 1, 1 }
			};

			rawModel = Loader::makeRawModel(Text::vertices, { 0, 1, 2, 2, 3, 0 });
			rawModel->loadTextureCoordinates(tex);

			initialized = true;
		}

		setLocation({ 0, 0, 0 });

		model = new Model(rawModel);
		//model->setLocation({location.x, l});
	}

	Text::Text(Font* fnt) {
		font = fnt;
		
		Text();
	}

	Text::~Text() {
		delete model;
	}

	void Text::setFont(Font* fnt) {
		font = fnt;
	}

	void Text::setString(std::string s) {
		str = s;
	}

	void Text::setLocation(vec2f loc) {
		location = { loc.x, loc.y, 0 };
	}

	void Text::setLocation(vec3f loc) {
		location = loc;
	}

	vec3f Text::getLocation() const {
		return location;
	}

	Font* Text::getFont() const {
		return font;
	}

	std::string Text::getString() const {
		return str;
	}

	RawModel * Text::getRawModel() {
		return rawModel;
	}

	Model* Text::getModel() const {
		return model;
	}
}