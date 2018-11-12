#pragma once

#include "Def.h"
#include "Model.h"
#include "Shader.h"
#include "camera.h"

namespace Ansel
{
	class Renderer
	{
		friend class Engine;

		/**
		  * Holds all rendering information that the renderer
		  * does not exclusivly require.
		  */
		struct RenderSettings
		{
			int _3D = 0;		///< Render with 3D? does nothing right now
			bool wireframe = false; ///< Render with wireframe if true.
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

		static mat4x4 projection;		///< Projection matrix for projecting 3D objects

		static RenderSettings settings;		///< Instance of the RenderSettings for different options.
		static std::vector<Light> lights;	///< List of lights to render with.

		static Shader* shader;			///< Default shader.
		static void prepare();			///< Mostly unused function to clear the screen. <- UNUSED ->
		static unsigned int uFrame;		///< Ticker that counts frames. <- UNUSED ->
		

	public:
		static mat4x4 ANSEL_API view;		///< View matrix that represents the camera in 3D space.

		static void loadShader(Shader* shader);
		
		/**
		  * Initialize the renderer. (maybe make this private)
		  * @params dimensions (w, h) dimensions of the screen
		  */
		static void ANSEL_API init(vec2u dimensions);

		/**
		  * Render to the screen. <- UNUSED ->
		  * @param model Model object to render.
		  */
		static void ANSEL_API Render(Model* model);
		/**
		  * Render to the screen.
		  * @param rawModel RawModel object to render
		  * @param camera   Camera to render the RawModel with
		  * @param s        Shader to render the RawModel with (uses Renderer's by default)
		  */
		static void ANSEL_API Render(RawModel* rawModel, Camera camera, Shader* s = NULL);
		/**
		  * Render to the screen.
		  * @param model  Model object to render
		  * @param camera Camera to render the Model with
		  * @param s      shader to render the Model with (uses Renderer's by default)
		  */
		static void ANSEL_API Render(Model* model, Camera camera, Shader* s = NULL);
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

		static void ANSEL_API genProjection(float zNear, float zFar, float FOV, float aspectRatio);
		static void ANSEL_API set3D(bool isOn);
		static void ANSEL_API setWireFrame(bool isOn);
		static void ANSEL_API toggleWireFrame();
	};
}
