#include "../../headers/rendering/Renderer.h"

#include "../../headers/entities/Skybox.h"

#include <algorithm>
#include <thread>

#include <GL/glew.h>
//#include <ECS.h>

using namespace ECS;

namespace Ansel
{
	/* <----------  PRIVATE  ---------->*/

	Light Renderer::lights[LIGHT_COUNT];
	unsigned int Renderer::light_index = 0;

	mat4x4 Renderer::projection;

	Renderer::RenderSettings Renderer::settings;

	FrameBuffer* Renderer::frame;
	RawModel*    Renderer::frameModel;

	Shader* Renderer::shader;
	Shader* Renderer::frameShader;

	unsigned int Renderer::uFrame = 0;
	vec2u Renderer::dimensions;

	void Renderer::renderFrame() {
		// Always draw the quad as filled, the rendered scene, however
		// will still be wireframed
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Bind the frameShader for rendering the quad
		frameShader->bind();

		// Now, there are only two textures in the frame buffer:
		// the depth and color textures.
		
		// The textures are already bound to this model and will
		// get updated along with the frame buffer.
		for (int i = 0; i < frameModel->getTextureSize(); i++) {
			// Get texture i from the RawModel
			unsigned int texID = frameModel->getTexture(i)->getID();

			// Make the active texture i and bind
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texID);

			// Set the uniform variable of texturei equal to the texture location
			frameShader->setUniform(i, "texture" + std::to_string(i));
		}

		// Once everything's set up, bind the VAO
		frameModel->getVAO()->bind();

		// Draw the RawModel
		glDrawElements(GL_TRIANGLES, frameModel->getVertexCount(), GL_UNSIGNED_INT, 0);

		// Finally, unbind everything
		frameModel->getVAO()->unbind();
		frameShader->unbind();
	}
	
	/* <-----------  PUBLIC  ---------->*/

	void Renderer::init(Window* w) {
		// Initialize Skybox
		Skybox::box = Loader::makeRawModel("assets/models/cube.obj", "_skybox", false);

		Shader* skyshader = new Shader();

		// First make and compile the three shaders
		skyshader->makeShader("ansel/shaders/shader.vert", VERTEX);
		skyshader->makeShader("ansel/shaders/skybox.frag", FRAGMENT);
		skyshader->makeShader("ansel/shaders/shader.geo", GEOMETRY);

		skyshader->link();

		Material* material = new Material;
		material->loadShader(skyshader);

		Skybox::box->loadMaterial(material);

		dimensions = { w->getWidth(), w->getHeight() };

		/* Shaders */

		// Manually go through the steps of creating the default shader
		shader = new Shader();

		// First make and compile the three shaders
		shader->makeShader("ansel/shaders/shader.vert", VERTEX);
		shader->makeShader("ansel/shaders/shader.frag", FRAGMENT);
		shader->makeShader("ansel/shaders/shader.geo", GEOMETRY);

		// Then link them to the program
		shader->link();
		shader->unbind();

		// Repeat the same process for the frameShader
		frameShader = new Shader();

		frameShader->makeShader("ansel/shaders/frame.vert", VERTEX);
		frameShader->makeShader("ansel/shaders/frame.frag", FRAGMENT);

		frameShader->link();
		frameShader->unbind();

		// Generate the font's default shader
		Font::shader = new Shader();

		Font::shader->makeShader("ansel/shaders/frame.vert", VERTEX);
		Font::shader->makeShader("ansel/shaders/font.frag", FRAGMENT);

		Font::shader->link();
		Font::shader->unbind();

		/* Framebuffer */

		// Generate FrameBuffer object
		frame = new FrameBuffer(w);

		// Create vertices for the quad
		std::vector<vec3f> screenVertices = {
			{ -1.f, -1.f, 0.f },
			{  1.f,  1.f, 0.f },
			{  1.f, -1.f, 0.f },
			{ -1.f,  1.f, 0.f }
		};

		// Create the indices for rendering the quad
		std::vector<unsigned int> screenIndices = {
			0, 2, 1,
			1, 3, 0
		};

		// Create the texture coordinates for the textures
		// from the framebuffer
		std::vector<vec2f> screenTex = {
			{ 0, 0 },
			{ 1, 1 },
			{ 1, 0 },
			{ 0, 1 }
		};

		// Finally use the loader to make a raw model with the vertices and
		// the indices. Then load the texture and its respective coordinates
		frameModel = Loader::makeRawModel(screenVertices, screenIndices);
		frameModel->loadTextureCoordinates(screenTex);
		frameModel->loadTexture(frame->getColorTexture());

		// Generate projection matrix
		genProjection(.005f, 1000.f, 65.f, (float)w->getHeight() / (float)w->getWidth());
	}

	/* Rendering */
	void Renderer::Render(EntityInstance entity, Camera camera, Shader* s) {
		auto components = entity->getComponents();

		for (int i = 0; i < components.size(); i++) {
			ComponentInstance component = components.at(i);
			
			int type = component->getType();
			if (component->getDerivative() != -1)
				type = component->getDerivative();

			switch (type) {
			case Component::SCRIPT:
			{
				Script* c = (Script*)component;
				if (c->updateable()) {
					c->update();
				}
				break;
			}
			case Component::CONTROLLER:
			{
				Controller* c = (Controller*)component;
				if (c->updateable()) {
					c->update(entity);
				}
				break;
			}
			case Component::RENDERABLE:
			{
				Model* model = component->cast<Model*>();
				Render(model, camera, s);
				break;
			}

			default:
				break;
			}

		}
	}

	void Renderer::processEntities(const std::vector<ECS::Entity*> &entities, std::vector<Model*> *models) {
		for (int i = 0; i < entities.size(); i++) {
			ECS::Entity* entity = entities.at(i);
			auto components = entity->getComponents();

			for (int i = 0; i < components.size(); i++) {
				ComponentInstance component = components.at(i);

				int type = component->getType();
				if (component->getDerivative() != -1)
					type = component->getDerivative();

				switch (type) {
				case Component::SCRIPT:
				{
					Script* c = (Script*)component;
					if (c->updateable()) {
						c->update();
					}
					break;
				}
				case Component::CONTROLLER:
				{
					Controller* c = (Controller*)component;
					if (c->updateable()) {
						c->update(entity);
					}
					break;
				}
				case Component::RENDERABLE:
				{
					models->push_back(component->cast<Model*>());
					break;
				}

				default:
					break;
				}

			}
		}
	}

	void Renderer::Render(const std::vector<ECS::Entity*> &entities, Camera camera, Shader* s) {
		const unsigned int max = 200;
		std::vector<std::vector<Model*>*> models;

		std::vector<std::thread*> threads;
		std::vector<ECS::Entity*> subEntities;

		// Go through the list and update each entities components
		// as well as collect the models for rendering
		unsigned int block_index = 0;
		for (int i = 0; i < entities.size(); i++) {
			if (i - block_index >= max || i == entities.size() - 1) {
				std::vector<ECS::Entity*> subList(entities.begin() + block_index, entities.begin() + i + 1);
				models.push_back(new std::vector<Model*>);

				threads.push_back(new std::thread(Renderer::processEntities, subList, models.at(models.size() - 1)));
				block_index = i;
			}
		}
		
		std::vector<Model*> finalModels;
		for (int i = 0; i < threads.size(); i++) {
			threads.at(i)->join();
			finalModels.insert(finalModels.begin(), models.at(i)->begin(), models.at(i)->end());
		}

		Render(finalModels, camera, s, 0);

		for (int i = 0; i < threads.size(); i++) {
			delete models.at(i);
			delete threads.at(i);
		}
	}

	void Renderer::Render(Text* text, Camera camera, Shader* s) {
		Shader* current_shader = (s == nullptr) ? Font::shader : s;

		current_shader->bind();

		VAO* vao = text->getRawModel()->getVAO();

		float x = text->getModel()->getLocation().x;
		float y = text->getModel()->getLocation().y;

		int index = 0;
		std::string str = text->getString();
		for (char c : str) {
			if (c == ' ') {
				index++;
				x += 1.5f;
				continue;
			}


			Character ch = text->getFont()->getCharacters()[c];
			
			vec4f location;

			vec4f rotation = {
				text->getModel()->getRotation().x,
				text->getModel()->getRotation().y,
				text->getModel()->getRotation().z,
				1.f
			};

			vec4f scale = {
				ch.Size.x * text->getModel()->getScale().x / 8.f,
				ch.Size.y * text->getModel()->getScale().y / 8.f,
				text->getModel()->getScale().z,
				1.f
			};

			location.x = x + (ch.Bearing.x * scale.x);
			location.y = y - ((ch.Size.y - ch.Bearing.y) * scale.y);

			text->getRawModel()->loadTransformations(
				{ location },
				{ rotation },
				{ scale }
			);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			
			vao->bind();

			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, 1);

			vao->unbind();

			x += 1.5f;
			index++;

		}

		current_shader->unbind();
	}

	void Renderer::Render(RawModel* rawModel, Camera camera, Shader* s) {
		// Make a temporary model from the RawModel
		Model* model = new Model(rawModel);

		// Render the model
		Render(model, camera, s);

		// Delete the model
		delete model;
	}

	void Renderer::Render(Model* model, Camera camera, Shader* s) {
		// Push the model to a vector of models
		std::vector<Model*> models;
		models.push_back(model);

		// Render the vector of models
		Render(models, camera, s);
	}

	void Renderer::Render(const std::vector<Model*> &models, Camera camera, Shader* s, int layer) {

		// If the layer is 0 (which has to be specified) then
		// Sort the list based off the RawModel IDs and recursivly
		// render
		if (layer == 0) {

			//std::sort(models.begin(), models.end(), [](Model* m1, Model* m2) {
			//	if (m1->getRawModel()->getID() > m2->getRawModel()->getID())
			//		return true;
			//
			//	return false;
			//});

			std::vector<std::vector<Model*>> temp_list(RawModel::count);
			int current_id = -1;
			for (int i = 0; i < models.size(); i++) {
				current_id = models.at(i)->getRawModel()->getID();

				temp_list.at(current_id).push_back(models.at(i));
			}

			for (int i = 0; i < temp_list.size(); i++) {
				if (temp_list.at(i).size() > 0)
					Render(temp_list.at(i), camera, s);
			}

			return;
		}

		// Make vectors of the models lcoations, scales, and rotations
		std::vector<vec4f> locations, scales, rotations, colors;

		// Loop through the models to accumulate their information
		for (int i = 0; i < models.size(); i++) {
			vec4f v = models.at(i)->getLocation();
			locations.push_back(v);

			vec3f s = models.at(i)->getScale();
			scales.push_back({ s.x, s.y, s.z, 1 });

			vec3f r = models.at(i)->getRotation();
			rotations.push_back({ r.x, r.y, r.z, 1 });

			vec4f c = models.at(i)->getColor();
			colors.push_back(c);
		}

		// Render the list of models with their respective transformation information
		Renderer::Render(models, locations, scales, rotations, colors, camera, s);
	}

	void Renderer::Render(ParticleSystem* particleSystem, Camera camera, Shader* s) {
		if (particleSystem->getParticleSize() == 0) return;

		std::vector<vec4f> locations, rotations, scales, colors;

		locations.resize(particleSystem->getParticleSize());
		rotations.resize(particleSystem->getParticleSize());
		scales   .resize(particleSystem->getParticleSize());
		colors   .resize(particleSystem->getParticleSize());

		for (int i = 0; i < particleSystem->getParticleSize(); i++) {
			PSys::Particle* p = particleSystem->getParticle(i);

			locations.at(i) = { p->location.x, p->location.y, p->location.z, 1.f };
			rotations.at(i) = { 0, 0, 0 };
			scales   .at(i) = { .01f, .01f, .01f };
			colors   .at(i) = p->c_color;
		}

		Model* model = new Model(particleSystem->getModel());

		std::vector<Model*> models;
		models.push_back(model);

		Renderer::Render(models, locations, scales, rotations, colors, camera);

		delete model;
	}

	void Renderer::Render(StaticModelArray SMA, Camera camera, Shader* s) {
		//Render(SMA.getModels(), SMA.getTransformation(SMA.LOCATIONS), SMA.getTransformation(SMA.SCALES),
		//	SMA.getTransformation(SMA.ROTATION), camera, s);
	}

	void Renderer::Render(const std::vector<Model*> &models, const std::vector<vec4f> &locations, const std::vector<vec4f> &scales, const std::vector<vec4f> &rotations, const std::vector<vec4f> &colors,
		Camera camera, Shader* s) {

		if (models.at(0) == nullptr) return;

		// If wireframe mode is on, tell OpenGL to render lines, 
		// otherwise fill in the polygons
		if (settings.wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		// If the shader passed to this function is null, use the 
		// default shader, otherwise use s
		Shader* current_shader = (s == NULL) ? shader : s;

		// For my convienice make a pointer from the model's RawModel
		RawModel* rawModel = models.at(0)->getRawModel();
		// Do the same for the VAO
		VAO *vao = rawModel->getVAO(); 
		
		rawModel->loadColors(colors);

		// Use the material shader (if there is one) over all others
		if (rawModel->getMaterial() != nullptr && rawModel->getMaterial()->getShader() != nullptr)
			current_shader = rawModel->getMaterial()->getShader();

		// Bind the shader
		current_shader->bind();

		// Set all the uniforms that can be used in the shader
		current_shader->setUniform((float)uFrame    , "frame"   );
		current_shader->setUniform((int)settings.lighting, "lighting");

		current_shader->setUniform((int)rawModel->colorsOn( )  , "use_colors"  );
		current_shader->setUniform((int)rawModel->normalsOn( ) , "use_normals" );
		current_shader->setUniform((int)rawModel->texturesOn( ), "use_textures");

		current_shader->setUniform(projection      , "projection");
		current_shader->setUniform(camera.getView(), "view"      );

		current_shader->setUniform(camera.getLocation(), "camera_position");

		if (rawModel->getMaterial() != nullptr) {
			current_shader->setUniform(rawModel->getMaterial()->getSpecStrength(), "mat_spec");
			current_shader->setUniform(rawModel->getMaterial()->getAmbientColor(), "mat_ambient");
			current_shader->setUniform(rawModel->getMaterial()->getEmissionStrength(), "mat_emissionStrength");
			current_shader->setUniform(rawModel->getMaterial()->getEmissionColor(), "mat_emissionColor");

			if (rawModel->getMaterial()->getTextureSize() > 0)
				current_shader->setUniform(1, "use_textures");
		}

		// Pass the lights to the uniform
		for (int i = 0; i < LIGHT_COUNT; i++) {
			Light light = lights[i];

			current_shader->setUniform(light.on       , "lights[" + std::to_string(i) + "].on"       );
			current_shader->setUniform(light.type     , "lights[" + std::to_string(i) + "].type"     );
			current_shader->setUniform(light.color    , "lights[" + std::to_string(i) + "].color"    );
			current_shader->setUniform(light.location , "lights[" + std::to_string(i) + "].location" );
			current_shader->setUniform(light.intensity, "lights[" + std::to_string(i) + "].intensity");
		}

		unsigned int material_amount = 0;
		for (int i = 0; i < Material::AMOUNT; i++) {
			std::string strings[Material::AMOUNT] = {
				"tex_albedo",
				"tex_normal",
				"tex_occlusion",
				"tex_emission"
			};

			if (rawModel->getMaterial() != nullptr && rawModel->getMaterial()->getTexture(i) != nullptr) {
				glActiveTexture(GL_TEXTURE0 + i);

				unsigned int texID = rawModel->getMaterial()->getTexture(i)->getID();
				glBindTexture(GL_TEXTURE_2D, texID);
				current_shader->setUniform(i, strings[i]);
				material_amount++;
			}
		}

		// Loop through the rawModel's textures and bind them to the shader 
		for (int i = 0; i < rawModel->getTextureSize(); i++) {
			glActiveTexture(GL_TEXTURE0 + i + material_amount + 1);

			unsigned int texID = rawModel->getTexture(i)->getID();
			glBindTexture(GL_TEXTURE_2D, texID);
			current_shader->setUniform(i + (int)material_amount + 1, "texture" + std::to_string(i));
		}

		// Finally, load the transformation information to the shader
		rawModel->loadTransformations(locations, rotations, scales);

		// Bind the VAO
		vao->bind();

		// Bind the Framebuffer so the rendering goes to that rather than
		// the window
		frame->bind();
		glDrawElementsInstanced(GL_TRIANGLES, rawModel->getVertexCount(), GL_UNSIGNED_INT, 0, locations.size());
		frame->unbind();

		// Unbind everything once and for all
		vao->unbind();
		rawModel->unbindTexures();
		current_shader->unbind();
	}

	/* Setting Defaults */

	void Renderer::loadShader(Shader* s) {
		shader = s;
	}

	void Renderer::loadFrameShader(Shader* s) {
		frameShader = s;
	}

	Shader* Renderer::getFrameShader() {
		return frameShader;
	}

	int Renderer::makeLight(Light light) {
		int i = light_index;

		lights[light_index++] = light;

		return i;
	}

	Light* Renderer::getLight(const unsigned int index) {
		return &lights[index];
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

	/* Setters */

	void Renderer::setWireFrame(bool isOn) {
		settings.wireframe = isOn;
	}

	void Renderer::setLighting(bool isOn) {
		settings.lighting = isOn;
	}

	void Renderer::toggleWireFrame() {
		if (settings.wireframe == true) {
			settings.wireframe = false;
		}
		else {
			settings.wireframe = true;
		}
	}
	
	/* Getters */

	FrameBuffer* Renderer::getFrameBuffer() {
		return frame;
	}

}