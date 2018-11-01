#pragma once

#include "Def.h"
#include "util.h"

#include <vector>
#include <GLFW/glfw3.h>

namespace Ansel
{
	class ANSEL_API Screen
	{
		Screen* nextScreen = NULL;

	protected:
		struct ANSEL_API Draw {
			Draw();

			void line(Ansel::vec2f startpoint, Ansel::vec2f endpoint, Ansel::vec4f color = { 1, 1, 1, 1 }) const;
			void lines(std::vector<Ansel::vec2f> points, Ansel::vec4f color = { 1, 1, 1, 1 }) const;

			void point(Ansel::vec2f location, float size = 5.f, Ansel::vec4f color = { 1, 1, 1, 1 }) const;

			void triangle(Ansel::vec2f p1, Ansel::vec2f p2, Ansel::vec2f p3, Ansel::vec4f color = { 1, 1, 1, 1 }, bool filled = true) const;
			void triangles(std::vector<Ansel::vec2f> points, Ansel::vec4f color = { 1, 1, 1, 1 }, bool filled = true) const;

			void rectangle(Ansel::FloatRect rect, Ansel::vec4f color = { 1, 1, 1, 1 }, bool filled = true);

			void circle(Ansel::vec2f location, float radius, unsigned int nodes = 10, Ansel::vec4f color = { 1, 1, 1, 1 }, bool filled = true) const;
		};

		Draw draw;
		unsigned int uFrame = 0;

		void setNextScreen(Screen* screen);

	public:
		Screen();

		virtual void onUpdate() = 0;

		virtual void onCreate();
		virtual void onDestroy();

		Screen* getNextScreen();
	};
}