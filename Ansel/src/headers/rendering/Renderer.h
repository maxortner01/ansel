#pragma once

#include "../Def.h"
#include "../entities/Model.h"
#include "../entities/camera.h"
#include "../entities/ParticleSystem.h"
#include "../entities/StaticModelArray.h"

#include "FrameBuffer.h"
#include "Shader.h"
#include "Text.h"

#include <vector>

//#ifndef __CLIENT_ECS
//	namespace ECS
//	{
//		class Entity;
//		class Component;
//	}
//#else
#	include <ECS.h>
//#endif // __CLIENT_ECS


namespace Ansel
{
	/**
	  * Object that represents a light in 3D space.
	  * @param on=false        whether the light is on or off
	  * @param type=NONE       the type of light
	  * @param color=white     the color of the light
	  * @param location=0,0,0  the location of the light
	  */
	struct Light
	{
		typedef enum {
			AMBIENT,
			POINT,
			DIRECTIONAL,
			NONE
		} LIGHT_TYPE;

		bool       on   = false;
		LIGHT_TYPE type = NONE;
		float intensity = -1.f;

		vec3f color = { 1, 1, 1 };
		vec4f location = { 0, 0, 0, 1 };
	};

	/**
	  * \brief Controller for all things rendering. 
	  *
	  * Anything that gets rendered to the screen goes through this class
	  * first. All of its members are static and thus does not need to be
	  * instanciated by the user or engine.
	  *
	  */
	class Renderer
	{
		friend class Engine;

		/**
		  * Holds all rendering information that the renderer
		  * does not exclusivly require.
		  */
		struct RenderSettings
		{
			bool wireframe = false; ///< Render with wireframe if true.
			bool lighting  = true;	///< Controls whether or not the normals effect the fragment color.
		};
		
		static Light  lights[LIGHT_COUNT];	///< List of lights to render with.
		static unsigned int light_index;

		static mat4x4 projection;			///< Projection matrix for projecting 3D objects

		static RenderSettings settings;		///< Instance of the RenderSettings for different options.

		static FrameBuffer* frame;			///< Framebuffer object that contains depth and color textures.
		static RawModel*    frameModel;		///< Simple square model for drawing frame to the screen.

		static Shader* shader;				///< Default shader.
		static Shader* frameShader;			///< Shader for drawing frame.

		static unsigned int uFrame;			///< Ticker that counts frames.
		static vec2u dimensions;

		/**
		  * \brief Renders the framebuffer to the screen.
		  *
		  * After all has been rendered through the screen, the framebuffer is passed
		  * through the framShader and finally rendered to the screen. Any UI drawing
		  * occurs after this in the engine. 
		  *
		  * The frameShader contains any post-processing effects.
		  */
		static void renderFrame();	

	public:
		/**
		  * Initialize the renderer. (maybe make this private)
		  * @params dimensions (w, h) dimensions of the screen
		  */
		static void ANSEL_API init(Window* w);

		/// Component System rendering
		static void ANSEL_API Render(ECS::Entity* entity, Camera camera = Camera(), Shader* s = nullptr);

		static void ANSEL_API Render(std::vector<ECS::Entity*> entities, Camera camera = Camera(), Shader* s = nullptr);

		static void ANSEL_API Render(Text* text, Camera camera = Camera(), Shader* s = nullptr);

		/**
		  * Render to the screen.
		  * @param rawModel RawModel object to render
		  * @param camera   Camera to render the RawModel with
		  * @param s        Shader to render the RawModel with (uses Renderer's by default)
		  */
		static void ANSEL_API Render(RawModel* rawModel, Camera camera = Camera() , Shader* s = nullptr);
		/**
		  * Render to the screen.
		  * @param model  Model object to render
		  * @param camera Camera to render the Model with
		  * @param s      shader to render the Model with (uses Renderer's by default)
		  */
		static void ANSEL_API Render(Model* model, Camera camera = Camera(), Shader* s = nullptr);
		/**
		  * Render to the screen. Precalculates translations, rotations, and scales from
		  * the list of models. This can be cumbersome if the size of models gets very large.
		  * @param models List of models to render
		  * @param camera Camera to render the models with
		  * @param s      Shader to render the models with (uses Renderer's by default)
		  */
		static void ANSEL_API Render(std::vector<Model*> models, Camera camera = Camera(), Shader* s = nullptr, int layer = 1);

		static void ANSEL_API Render(ParticleSystem* particleSystem, Camera camera = Camera(), Shader* s = nullptr);

		static void ANSEL_API Render(StaticModelArray staticModelArray, Camera camera = Camera(), Shader* s = nullptr);

		/**
		  * Render to the screen. Manually inputted translation, rotation, and scale information.
		  * Ideally, the size of locations, scales, and rotations is the same as models.
		  * @param models    List of models to render
		  * @param locations Each model's location
		  * @param scales    Each model's scale
		  * @param rotations Each model's rotation
		  */
		static void ANSEL_API Render(std::vector<Model*> models, std::vector<vec4f> locations, std::vector<vec4f> scales, std::vector<vec4f> rotations, std::vector<vec4f> colors, Camera camera = Camera(), Shader* s = nullptr);

		/**
		  * Sets the default rendering shader.
		  * @param shader Shader instance to make default
		  */
		static void ANSEL_API loadShader(Shader* shader);
		
		/**
		  * Sets the default frame rendering shader.
		  * @param shader Shader instance to make the default frame shader
		  */
		static void ANSEL_API loadFrameShader(Shader* shader);
		
		/**
		  * Gets the current default frame shader.
		  */
		static Shader ANSEL_API * getFrameShader();
		
		/**
		  * Pushes light to the renderer. 
		  * @param  light Light to pass to the renderer
		  * @return Index of the passed light.
		  */
		static int   ANSEL_API   makeLight(Light light);
		
		/**
		  * Gets the light instance at a given index.
		  * @param index Index where the desired light is located.
		  */
		static Light ANSEL_API * getLight (const unsigned int index);

		/**
		  * Generates the default projection matrix with the given parameters.
		  * @param zNear       Close clipping plane.
		  * @param zFar        Far clipping plane.
		  * @param FOV         Field-of-view
		  * @param aspectRatio Height of the window / width of the window
		  */
		static void ANSEL_API genProjection(float zNear, float zFar, float FOV, float aspectRatio);

		/**
		  * Sets the wireframe setting.
		  * @param 
		  */
		static void ANSEL_API setWireFrame(bool isOn);
		static void ANSEL_API setLighting(bool isOn);
		static void ANSEL_API toggleWireFrame();

		static FrameBuffer ANSEL_API * getFrameBuffer();
	};
}
