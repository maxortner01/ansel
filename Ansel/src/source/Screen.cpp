#include "../headers/Screen.h"

namespace Ansel
{
	Screen::Screen(Window* w) { 
		window = w;

	}

	void Screen::onCreate() {}
	void Screen::onDestroy() {}

	void Screen::setNextScreen(Screen* screen) {
		nextScreen = screen;
	}

	Screen* Screen::getNextScreen() {
		return nextScreen;
	}
	
	// DRAWING
	Screen::Draw::Draw() {}

	void Screen::Draw::line(Ansel::vec2f startpoint, Ansel::vec2f endpoint, Ansel::vec4f color) const {
		glBegin(GL_LINES);
		{
			// Set color
			glColor4f(color.r, color.g, color.b, color.a);
			// Define start point
			glVertex2f(startpoint.x, startpoint.y);
			// Define end point
			glVertex2f(endpoint.x, endpoint.y);
		}
		glEnd();
	}

	void Screen::Draw::lines(std::vector<Ansel::vec2f> points, Ansel::vec4f color) const {
		glBegin(GL_LINES);
		{
			// Set color
			glColor4f(color.r, color.g, color.b, color.a);

			// Iterate through all the points and render
			for (Ansel::vec2f p : points)
				glVertex2f(p.x, p.y);
		}
		glEnd();
	}

	void Screen::Draw::point(Ansel::vec2f location, float size, Ansel::vec4f color) const {
		// Set point size
		glPointSize(size);

		glBegin(GL_POINTS);
		{
			// Set color
			glColor4f(color.r, color.g, color.b, color.a);
			// Set point location
			glVertex2f(location.x, location.y);
		}
		glEnd();
	}

	void Screen::Draw::points(std::vector<Ansel::vec2f> locations, std::vector<Ansel::vec4f> colors, float size) const {
		glPointSize(size);
		
		glBegin(GL_POINTS);
		{
			for (int i = 0; i < locations.size(); i++) {
				Ansel::vec2f location = locations.at(i);
				Ansel::vec4f color;

				if (i > colors.size() - 1) color = colors.at(colors.size() - 1);
				else                       color = colors.at(i);

				glColor4f(color.r, color.g, color.b, color.a);
				glVertex2f(location.x, location.y);
			}		
		}
		glEnd();
	}

	void Screen::Draw::triangle(Ansel::vec2f p1, Ansel::vec2f p2, Ansel::vec2f p3, Ansel::vec4f color, bool filled) const {
		glBegin(GL_TRIANGLES);
		{
			glColor4f(color.r, color.g, color.b, color.a);
			glVertex2f(p1.x, p1.y);
			glVertex2f(p2.x, p2.y);
			glVertex2f(p3.x, p3.y);
		}
		glEnd();
	}

	void Screen::Draw::triangles(std::vector<Ansel::vec2f> points, Ansel::vec4f color, bool filled) const {
		glBegin(GL_TRIANGLES);
		{
			glColor4f(color.r, color.g, color.b, color.a);

			for (Ansel::vec2f p : points)
				glVertex2f(p.x, p.y);
		}
		glEnd();
	}

	void Screen::Draw::rectangle(Ansel::FloatRect rect, Ansel::vec4f color, bool filled) {
		if (filled)
			glBegin(GL_TRIANGLES);
		else
			glBegin(GL_LINES);

		{
			glColor4f(color.r, color.g, color.b, color.a);

			glVertex2f(rect.x, rect.y);
			glVertex2f(rect.x + rect.w, rect.y);
			glVertex2f(rect.x, rect.y + rect.h);

			glVertex2f(rect.x + rect.w, rect.y + rect.h);
			glVertex2f(rect.x + rect.w, rect.y);
			glVertex2f(rect.x, rect.y + rect.h);
		}
		glEnd();

	}

	void Screen::Draw::circle(Ansel::vec2f location, float radius, unsigned int nodes, Ansel::vec4f color, bool filled) const {
		float precision = 360.f / (float)nodes;

		std::vector<Ansel::vec2f> points;
		for (unsigned int i = 0; i < nodes; i++) {
			const Ansel::vec2f nodeA = {
				radius * cosf(precision * i * (3.14159f / 180.f)) + location.x,
				radius * sinf(precision * i * (3.14159f / 180.f)) + location.y
			};

			const Ansel::vec2f nodeB = {
				radius * cosf(precision * (i + 1) * (3.14159f / 180.f)) + location.x,
				radius * sinf(precision * (i + 1) * (3.14159f / 180.f)) + location.y
			};

			points.push_back(nodeA);
			points.push_back(nodeB);
		}

		if (!filled)
			lines(points, color);
		else {
			std::vector<Ansel::vec2f> newPoints;
			for (int i = 0; i < points.size() / 2; i++) {
				newPoints.push_back(points.at(i * 2));
				newPoints.push_back(points.at(i * 2 + 1));
				newPoints.push_back({ location.x, location.y });
			} 

			triangles(newPoints, color);
		}
	}

}