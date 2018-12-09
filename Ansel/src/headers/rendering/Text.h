#pragma once

#include "../Def.h"
#include "../util.h"

#include "../rendering/Font.h"

#include "../entities/Model.h"

#include <vector>

namespace Ansel
{
	class Text
	{
		// For rendering a square
		static std::vector<vec3f> vertices;
		static RawModel* rawModel;

		static bool initialized;

		Model* model;
		Font* font;

		std::string str;
		vec3f location;

	public:
		ANSEL_API Text();
		ANSEL_API Text(Font* fnt);

		ANSEL_API ~Text();

		void ANSEL_API setFont(Font* fnt);
		void ANSEL_API setString(std::string s);

		void ANSEL_API setLocation(vec2f loc);
		void ANSEL_API setLocation(vec3f loc);

		vec3f ANSEL_API   getLocation() const;
		Font  ANSEL_API * getFont()     const;

		std::string  ANSEL_API getString() const;

		static RawModel ANSEL_API * getRawModel();
		Model     ANSEL_API * getModel()    const;
	};
}