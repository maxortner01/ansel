#include "../../headers/rendering/Renderer.h"

#include <GL/glew.h>
#include <ECS.h>

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
				Script* s = (Script*)component;
				if (s->updateable()) {
					s->update();
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

	void Renderer::Render(Text* text, Camera camera, Shader* s) {
		Shader* current_shader = (s == nullptr) ? shader : s;

		current_shader->bind();

		VAO* vao = text->getRawModel()->getVAO();

		std::string str = text->getString();
		for (char c : str) {
			Character ch = text->getFont()->getCharacters()[c];

			float xpos = text->getLocation().x + ch.Bearing.x /* * get_scale() */;
			float ypos = text->getLocation().y - (ch.Size.y - ch.Bearing.y) /* * get_scale() */;

			float w = ch.Size.x /* * get_scale() */;
			float h = ch.Size.y /* * get_scale() */;

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			
			vao->bind();

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			vao->unbind();
			
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

	void Renderer::Render(std::vector<Model*> models, Camera camera, Shader* s, int layer) {

		if (layer == 0) {


			return;
		}

		// Make vectors of the models lcoations, scales, and rotations
		std::vector<vec4f> locations, scales, rotations;

		// Loop through the models to accumulate their information
		for (int i = 0; i < models.size(); i++) {
			vec4f v = models.at(i)->getLocation();
			locations.push_back(v);

			vec3f s = models.at(i)->getScale();
			scales.push_back({ s.x, s.y, s.z, 1 });

			vec3f r = models.at(i)->getRotation();
			rotations.push_back({ r.x, r.y, r.z, 1 });
		}

		// Render the list of models with their respective transformation information
		Renderer::Render(models, locations, scales, rotations, camera, s);
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

		particleSystem->getModel()->loadColors(colors);

		Model* model = new Model(particleSystem->getModel());

		std::vector<Model*> models;
		models.push_back(model);

		Renderer::Render(models, locations, scales, rotations, camera);

		delete model;
	}

	void Renderer::Render(StaticModelArray SMA, Camera camera, Shader* s) {
		Render(SMA.getModels(), SMA.getTransformation(SMA.LOCATIONS), SMA.getTransformation(SMA.SCALES),
			SMA.getTransformation(SMA.ROTATION), camera, s);
	}

	void Renderer::Render(std::vector<Model*> models, std::vector<vec4f> locations, std::vector<vec4f> scales, std::vector<vec4f> rotations,
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
				"tex_occlusion"
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