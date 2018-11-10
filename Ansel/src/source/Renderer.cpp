#include "../headers/Renderer.h"

namespace Ansel
{
	mat4x4 Renderer::view;

	mat4x4 Renderer::projection;

	Renderer::RenderSettings Renderer::settings;

	Shader* Renderer::shader;

	std::vector<Renderer::Light> Renderer::lights;

	unsigned int Renderer::uFrame = 0;

	void Renderer::prepare() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::init(vec2u dimensions) {
		Shader* shader = new Shader();

		shader->makeShader("assets/shaders/shader.vert", VERTEX);
		shader->makeShader("assets/shaders/shader.frag", FRAGMENT);
		shader->makeShader("assets/shaders/shader.geo", GEOMETRY);

		shader->link();

		loadShader(shader);

		genProjection(.005f, 1000.f, 65.f, (float)dimensions.y / (float)dimensions.x);

		view.m[0][0] = 1.f;
		view.m[1][1] = 1.f;
		view.m[2][2] = 1.f;
		view.m[3][3] = 1.f;

		lights.resize(LIGHT_COUNT);
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

	void Renderer::Render(Model* model) {
		std::vector<vec4f> locations(INSTANCE_COUNT);
		locations.at(0) = model->getLocation();
		
		shader->bind();

		shader->setUniform((float)uFrame, "frame");
		shader->setUniform(locations, "location", INSTANCE_COUNT);

		RawModel* rawModel = model->getRawModel();
		
		VAO *vao = rawModel->getVAO();
		vao->bind();
		
		if (vao->bufferExists(Buffer::INDICES)) {
			vao->getBuffer(Buffer::INDICES).bind();

			glDrawElements(GL_TRIANGLES, rawModel->getVertexCount(), GL_UNSIGNED_INT, (void*)0);
		}
		else
			glDrawArrays(GL_TRIANGLES, 0, 3);

		vao->unbind();
	}
	
	void Renderer::Render(RawModel* rawModel, Camera camera, Shader* s) {
		Model* model = new Model(rawModel);

		Render(model, camera, s);

		delete model;
	}

	void Renderer::Render(Model* model, Camera camera, Shader* s) {
		std::vector<Model*> models;
		models.push_back(model);

		Render(models, camera, s);
	}

	void Renderer::Render(std::vector<Model*> models, Camera camera, Shader* s) {
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		std::vector<vec4f> locations, scales, rotations, lights;
		std::vector<int> light_states;

		Shader* current_shader = (s == NULL) ? shader : s;

		for (int i = 0; i < models.size(); i++) {
			vec4f v = models.at(i)->getLocation();
			locations.push_back(v); 

			vec3f s = models.at(i)->getScale();
			scales.push_back({ s.x, s.y, s.z, 1 });

			vec3f r = models.at(i)->getRotation();
			rotations.push_back({ r.x, r.y, r.z, 1 });
		}

		lights.push_back({ -1, .5f, 0, 1 });
		light_states.push_back(1);

		//TEMPORARY
		for (int i = 0; i < LIGHT_COUNT - 1; i++) {
			lights.push_back({ 0, 0, 0, 1 });
			light_states.push_back(0);
		}
		
		current_shader->setUniform((float)uFrame, "frame");
		current_shader->setUniform(settings._3D, "use3D");

		current_shader->setUniform(projection, "projection");
		current_shader->setUniform(camera.getView(), "view");

		current_shader->setUniform(light_states, "light_state", LIGHT_COUNT);
		current_shader->setUniform(lights, "light_position", LIGHT_COUNT);

		//shader->setUniform(locations, "location", INSTANCE_COUNT);

		current_shader->bind();

		RawModel* rawModel = models.at(0)->getRawModel();
		VAO *vao = rawModel->getVAO();

		rawModel->loadTransformations(locations, rotations, scales);

		vao->bind();
		
		glDrawElementsInstanced(GL_TRIANGLES, rawModel->getVertexCount(), GL_UNSIGNED_INT, 0, models.size());

		vao->unbind();
	}
	
	void Renderer::set3D(bool isOn) {
		settings._3D = isOn;
	}
}