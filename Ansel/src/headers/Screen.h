#pragma once

#include "Def.h"
#include "util.h"
#include "Window.h"

#include "rendering/Texture.h"

#include <vector>

namespace Ansel
{
	class ANSEL_API Screen
	{
		// Engine shares all parts of this class
		friend class Engine;

		float fps;
		Screen* nextScreen = NULL;

	protected:

		Window* window;

		struct ANSEL_API Draw {
			float aspectRatio = 1.f;

			Draw();

			void line(Ansel::vec2f startpoint, Ansel::vec2f endpoint, Ansel::vec4f color = { 1, 1, 1, 1 }) const;
			void lines(std::vector<Ansel::vec2f> points, Ansel::vec4f color = { 1, 1, 1, 1 }) const;

			void point(Ansel::vec2f location, float size = 1.f, Ansel::vec4f color = { 1, 1, 1, 1 }) const;
			void points(std::vector<Ansel::vec2f> locations, std::vector<Ansel::vec4f> colors, float size = 1.f) const;

			void triangle(Ansel::vec2f p1, Ansel::vec2f p2, Ansel::vec2f p3, Ansel::vec4f color = { 1, 1, 1, 1 }, bool filled = true) const;
			void triangles(std::vector<Ansel::vec2f> points, Ansel::vec4f color = { 1, 1, 1, 1 }, bool filled = true) const;

			void rectangle(Ansel::FloatRect rect, Ansel::vec4f color = { 1, 1, 1, 1 }, bool filled = true);

			void circle(Ansel::vec2f location, float radius, unsigned int nodes = 10, Ansel::vec4f color = { 1, 1, 1, 1 }, bool filled = true) const;

			void texture(Ansel::FloatRect rect, Texture* texture) const;
		};

		Draw draw;
		unsigned int uFrame = 0;
		vec2u ScreenSize;

		void setNextScreen(Screen* screen);
		float getFPS() const;

	public:

		Screen(Window* w);

		virtual void onUpdate() = 0;

		virtual void renderUI(float aspectRatio);
		virtual void onCreate();
		virtual void onDestroy();

		Screen* getNextScreen();
	};
}