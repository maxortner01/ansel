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
		genProjection(.1f, 1000.f, 65.f, (float)dimensions.y / (float)dimensions.x);

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
		std::vector<vec4f> locations;

		for (int i = 0; i < models.size(); i++) {
			vec4f v = models.at(i)->getLocation();
			locations.push_back(v);
		}
		
		shader->setUniform((float)uFrame, "frame");
		shader->setUniform(settings._3D, "use3D");

		shader->setUniform(projection, "projection");
		shader->setUniform(camera.getView(), "view");

		//shader->setUniform(locations, "location", INSTANCE_COUNT);

		shader->bind();

		RawModel* rawModel = models.at(0)->getRawModel();
		VAO *vao = rawModel->getVAO();

		rawModel->loadTransformations(locations);

		vao->bind();
		
		glDrawElementsInstanced(GL_TRIANGLES, rawModel->getVertexCount(), GL_UNSIGNED_INT, 0, models.size());

		vao->unbind();

		uFrame++;
	}
	
	void Renderer::set3D(bool isOn) {
		settings._3D = isOn;
	}
}