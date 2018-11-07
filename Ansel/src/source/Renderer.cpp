#include "../headers/Renderer.h"

namespace Ansel
{
	mat4x4 Renderer::view;

	mat4x4 Renderer::projection;

	Renderer::RenderSettings Renderer::settings;

	Shader* Renderer::shader;

	unsigned int Renderer::uFrame = 0;

	void Renderer::prepare() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::init(vec2u dimensions) {
		loadShader(new Shader("assets/shaders/shader.vert", "assets/shaders/shader.frag"));
		genProjection(.1f, 1000.f, 90.f, (float)dimensions.y / (float)dimensions.x);

		view.m[0][0] = 1.f;
		view.m[1][1] = 1.f;
		view.m[2][2] = 1.f;
		view.m[3][3] = 1.f;
	}

	void Renderer::loadShader(Shader* s) {
		shader = s;
	}

	void Renderer::genProjection(float zNear, float zFar, float FOV, float aspectRatio) {
		float fFovRad = 1.f / tanf(FOV * .5f / 180.f * 3.14159f);

		projection.m[0][0] = aspectRatio * fFovRad;
		projection.m[1][1] = fFovRad;
		projection.m[2][2] = zFar / (zFar - zNear);
		projection.m[2][3] = (-zFar * zNear) / (zFar - zNear);
		projection.m[3][2] = 1.f;
		projection.m[3][3] = 1.f;
	}

	void Renderer::Render(Model model) {
		std::vector<vec4f> locations(INSTANCE_COUNT);
		locations.at(0) = model.getLocation();
		
		shader->bind();

		shader->setUniform((float)uFrame, "frame");
		shader->setUniform(locations, "location", INSTANCE_COUNT);

		RawModel rawModel = *model.getRawModel();
		
		VAO *vao = rawModel.getVAO();
		vao->bind();
		
		if (vao->bufferExists(Buffer::INDICES)) {
			vao->getBuffer(Buffer::INDICES).bind();

			glDrawElements(GL_TRIANGLES, rawModel.getVertexCount(), GL_UNSIGNED_INT, (void*)0);
		}
		else
			glDrawArrays(GL_TRIANGLES, 0, 3);

		vao->unbind();

		uFrame++;
	}

	void Renderer::Render(std::vector<Model*> models, Camera camera) {
		std::vector<vec4f> locations(INSTANCE_COUNT);
		std::vector<vec4f> scales(INSTANCE_COUNT);

		for (int i = 0; (i < models.size() && i < INSTANCE_COUNT); i++) {
			vec4f v = models.at(i)->getLocation();
			locations.at(i) = v;

			vec3f s = models.at(i)->getScale();
			scales.at(i) = { s.x, s.y, s.z, 1 };
		}
		
		shader->setUniform((float)uFrame, "frame");
		shader->setUniform(settings._3D, "use3D");

		shader->setUniform(projection, "projection");
		shader->setUniform(camera.getView(), "view");

		shader->setUniform(locations, "location", INSTANCE_COUNT);
		shader->setUniform(scales, "scale", INSTANCE_COUNT);

		shader->bind();

		RawModel rawModel = *models.at(0)->getRawModel();
		VAO *vao = rawModel.getVAO();
		vao->bind();

		for (int i = 0; i < 16; i++)
			if (rawModel.getVAO()->bufferExists(i))
				glVertexAttribDivisor(i, 0);
		
		glDrawElementsInstanced(GL_TRIANGLES, rawModel.getVertexCount(), GL_UNSIGNED_INT, 0, models.size());

		for (int i = 0; i < 16; i++)
			if (rawModel.getVAO()->bufferExists(i))
				glVertexAttribDivisor(i, 0);

		vao->unbind();

		uFrame++;
	}
	
	void Renderer::set3D(bool isOn) {
		settings._3D = isOn;
	}
}