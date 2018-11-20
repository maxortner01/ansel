#pragma once

#include "Def.h"
#include "Model.h"
#include "Shader.h"
#include "camera.h"
#include "FrameBuffer.h"

namespace Ansel
{
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

		/**
		  * Object that represents a light in 3D space.
		  */
		struct Light
		{
			vec4f location;
			vec3f color;
			bool  on;
		};

		static mat4x4 projection;			///< Projection matrix for projecting 3D objects

		static RenderSettings settings;		///< Instance of the RenderSettings for different options.
		static std::vector<Light> lights;	///< List of lights to render with.

		static FrameBuffer* frame;			///< Framebuffer object that contains depth and color textures.
		static RawModel*    frameModel;		///< Simple square model for drawing frame to the screen.

		static Shader* shader;				///< Default shader.
		static Shader* frameShader;			///< Shader for drawing frame.

		static unsigned int uFrame;			///< Ticker that counts frames.

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

		/**
		  * Render to the screen.
		  * @param rawModel RawModel object to render
		  * @param camera   Camera to render the RawModel with
		  * @param s        Shader to render the RawModel with (uses Renderer's by default)
		  */
		static void ANSEL_API Render(RawModel* rawModel, Camera camera = Camera() , Shader* s = NULL);
		/**
		  * Render to the screen.
		  * @param model  Model object to render
		  * @param camera Camera to render the Model with
		  * @param s      shader to render the Model with (uses Renderer's by default)
		  */
		static void ANSEL_API Render(Model* model, Camera camera = Camera(), Shader* s = NULL);
		/**
		  * Render to the screen. Precalculates translations, rotations, and scales from
		  * the list of models. This can be cumbersome if the size of models gets very large.
		  * @param models List of models to render
		  * @param camera Camera to render the models with
		  * @param s      Shader to render the models with (uses Renderer's by default)
		  */
		static void ANSEL_API Render(std::vector<Model*> models, Camera camera = Camera(), Shader* s = NULL);
		/**
		  * Render to the screen. Manually inputted translation, rotation, and scale information.
		  * Ideally, the size of locations, scales, and rotations is the same as models.
		  * @param models    List of models to render
		  * @param locations Each model's location
		  * @param scales    Each model's scale
		  * @param rotations Each model's rotation
		  */
		static void ANSEL_API Render(std::vector<Model*> models, std::vector<vec4f> locations, std::vector<vec4f> scales, std::vector<vec4f> rotations, Camera camera = Camera(), Shader* s = NULL);

		static void ANSEL_API loadShader(Shader* shader);
		static void ANSEL_API loadFrameShader(Shader* shader);

		static void ANSEL_API genProjection(float zNear, float zFar, float FOV, float aspectRatio);

		static void ANSEL_API setWireFrame(bool isOn);
		static void ANSEL_API setLighting(bool isOn);
		static void ANSEL_API toggleWireFrame();

		static FrameBuffer ANSEL_API * getFrameBuffer();
	};
}
