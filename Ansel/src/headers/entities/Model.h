#pragma once

#include "../Def.h"
#include "../util.h"

#include "../rendering/Texture.h"
#include "../rendering/Material.h"

#include <vector>

#ifdef NO_ECS
	namespace ECS {
		class Entity;
	}
#else
#	include <ECS.h>
#endif

namespace Ansel
{
	/**
	  * VBO object for wrapping OpenGL structure.
	  */
	struct Buffer
	{
		enum TYPE {
			VERTEX,
			COLOR,
			NORMAL,
			TRANSFORMATION,
			ROTATION,
			SCALE,
			TEX_COORDS0,
			INDICES = 16
		} typedef BUFFER_TYPE;

		unsigned int ID = 0;
		unsigned int data_size = 0;
		unsigned int count = 0;
		BUFFER_TYPE type;

		void bind();
		void unbind();
	};

	/**
	  * Contains any relevant VBO objects, runs all Vertex Array Object functions
	  * and OpenGL calls.
	  */
	struct VAO
	{
		unsigned int nextAttribute = 0;
		unsigned int ID = 0;
		std::vector<Buffer> VBOS;

		VAO();

		static int getType(Buffer::BUFFER_TYPE type);

		void genBuffer(const Buffer::BUFFER_TYPE type);
		void bindBufferData(const std::vector<vec2f> data, Buffer::BUFFER_TYPE type = Buffer::BUFFER_TYPE::VERTEX, bool dynamic = false);
		void bindBufferData(const std::vector<vec3f> data, Buffer::BUFFER_TYPE type = Buffer::BUFFER_TYPE::VERTEX, bool dynamic = false);
		void bindBufferData(const std::vector<vec4f> data, Buffer::BUFFER_TYPE type = Buffer::BUFFER_TYPE::VERTEX, bool divisor = false, bool dynamic = false);
		void bindBufferData(const std::vector<int> data, 
			const unsigned int size = 2, const Buffer::BUFFER_TYPE type = Buffer::BUFFER_TYPE::VERTEX, bool dynamic = false);
		void bindBufferData(const std::vector<unsigned int> data,
			const unsigned int size = 2, const Buffer::BUFFER_TYPE type = Buffer::BUFFER_TYPE::VERTEX, bool dynamic = false);

		Buffer getBuffer(Buffer::BUFFER_TYPE type);
		bool   bufferExists(int type);

		void bind();
		void unbind();

		void destroy();
	};

	/**
	  * Holds the VAO and is only to be instantiated for every loaded model.
	  * Handles any low level buffer functions so the user doesn't need too.
	  * Ideally, the Loader::makeRawModel will be used rather than using this
	  * constructor.
	  */
	class RawModel
	{
		static unsigned int count;
		unsigned int ID;

		VAO *vao;			///< Pointer to the VAO object that defines this model
		unsigned int size;	///< Amount of vertices/indices
		unsigned int index;	///< Respective index inside the vector of RawModels
		bool use_textures = false;
		bool use_colors = false;
		bool use_normals = false;

		std::vector<Texture*> textures;
		Material* _material;

	public:
		/**
		  * Empty constructor that initializes and empty class.
		  */
		ANSEL_API RawModel();
		/**
		  * Constructor initializes the class.
		  * @param *v Pointer to a Vertex Array Object instance
		  * @param s  Amount of vertices
		  * @param i  Index within vector of RawModels (NULL if not using Loader)
		  */
		ANSEL_API RawModel(VAO *v, unsigned int s, unsigned int i);

		/**
		  * Loads specified colors into the corresponding color buffer.
		  * @param colors List of colors to add
		  */
		void ANSEL_API loadColors(std::vector<vec4f> colors);

		/**
		  * Loads texture coordinates into the buffer.
		  */
		void ANSEL_API loadTextureCoordinates(std::vector<vec2f> coords, unsigned int texture = 0);

		/**
		  * Creates texture.
		  */
		int ANSEL_API loadTexture(std::string filename);

		/**
		  * Creates texture.
		  */
		int ANSEL_API loadTexture(Texture* texture);

		void ANSEL_API loadMaterial(Material* material);

		/**
		  * Binds textures.
		  */
		void ANSEL_API bindTexures();

		/**
		  * Unbinds textures.
		  */
		void ANSEL_API unbindTexures();

		/*
		 * Loads specified normals into the corresponding normals buffer.
		 * @param normals List of (x, y, z) normals
		 */
		void ANSEL_API loadNormals(std::vector<vec3f> normals);
		/**
		  * Load transformation information for construction of the matrix in the shader.
		  * @param transformation (x, y, z, w) transformation information
		  * @param rotation Pitch, yaw, and roll rotation information
		  * @param scale (x, y, z) scale dimensions
		  */
		void ANSEL_API loadTransformations(std::vector<vec4f> transformation, std::vector<vec4f> rotation, std::vector<vec4f> scale);

		/**
		  * Get pointer to the corresponding VAO.
		  */
		VAO ANSEL_API * getVAO() const;
		/**
		  * Get the amount of vertices within this model.
		  */
		unsigned int ANSEL_API getVertexCount() const;
		/**
		  * Get the index within RawModel vector.
		  */
		unsigned int ANSEL_API getIndex() const;

		int  ANSEL_API colorsOn() const;
		int  ANSEL_API normalsOn() const;
		int  ANSEL_API texturesOn() const;

		void ANSEL_API setColorsOn(bool isOn);
		void ANSEL_API setNormalsOn(bool isOn);
		void ANSEL_API setTexturesOn(bool isOn);

		int ANSEL_API getTextureSize() const;

		Texture  ANSEL_API * getTexture(int index) const;
		Material ANSEL_API * getMaterial( ) const;

		ANSEL_API ~RawModel();
	};

	/**
	  * Contains only a pointer to its corresponding RawModel and transformation data.
	  * All models of a specific RawModel are grouped together and rendered at once
	  * with all transformation data being uploaded to the VAO every frame.
	  */
	class Model  
#ifdef __CLIENT_ECS
		: public ECS::Renderable
#endif
	{

#ifdef __CLIENT_ECS
		int __derivative = 0;
#endif // __CLIENT_ECS


		vec4f _location;	///< (x, y, z, w) location.
		vec3f _scale;		///< (x, y, z) scale factor.
		vec3f _rotation;	///< Pitch, yaw, and roll.
		vec4f _color;

		RawModel* _ref;		///< Pointer to RawModel instance that holds all vertex information.

	public:
		/**
		  * Constructs an empty model class, unusable.
		  */
		ANSEL_API Model();
		/**
		  * Constructs a model class with the corresponding vertex information
		  * coming from the RawModel.
		  * @param modelRef Pointer to the RawModel instance that defines all vertex information
		  */
		ANSEL_API Model(RawModel* modelRef);
		
		/**
		  * Virtual function that can be used 
		  */
		virtual void update() {}

		void ANSEL_API setLocation(vec4f location);
		void ANSEL_API setLocation(vec2f location);
		void ANSEL_API setLocation(float x, float y);
		void ANSEL_API setLocation(float x, float y, float z, float w);
		void ANSEL_API addLocation(float x, float y, float z, float w);

		void ANSEL_API setRotation(vec3f rotation);
		void ANSEL_API setRotation(float x, float y, float z);
		void ANSEL_API addRotation(float x, float y, float z);

		void ANSEL_API setColor(vec4f color);

		void ANSEL_API setScale(vec3f scale);

		vec4f	  ANSEL_API getLocation( )   const;
		vec3f	  ANSEL_API getScale( )      const;
		vec3f	  ANSEL_API getRotation( )   const;
		RawModel  ANSEL_API * getRawModel( ) const;
	};

	class Loader
	{
		friend class Engine;

		static std::vector<VAO*> vaos;

		static std::vector<std::string> names;
		static std::vector<RawModel*> rawModels;
		static std::vector<Model*> models;

		static std::vector<std::string> splitString(const char delimiter, std::string str);

		static int createVAO();

		static unsigned int nextModelIndex;

		static RawModel* readOBJ(const char* filename, const char* storage_name = "", bool smooth_shading = true);
		static RawModel* readFBX(const char* filename, const char* storage_name = "", bool smooth_shading = true);

	public:

		static ECS::Entity ANSEL_API * makeEntity(std::string name = "");
		static ECS::Entity ANSEL_API * getEntity (std::string name);

		static RawModel ANSEL_API * getRawModel(const char* storage_name);

		static RawModel ANSEL_API * makeRawModel(std::vector<vec2f> vertices, const char* storage_name = "");
		static RawModel ANSEL_API * makeRawModel(std::vector<vec3f> vertices, const char* storage_name = "");
		static RawModel ANSEL_API * makeRawModel(std::vector<vec2f> vertices, std::vector<unsigned int> indices, const char* storage_name = "");
		static RawModel ANSEL_API * makeRawModel(std::vector<vec3f> vertices, std::vector<unsigned int> indices, const char* storage_name = "");
		static RawModel ANSEL_API * makeRawModel(std::vector<vec3f> vertices, std::vector<unsigned int> indices, std::vector<vec3f> normals, const char* storage_name = "");
		static RawModel ANSEL_API * makeRawModel(const char* filename, const char* storage_name = "", bool smooth_shading = true);
		static Model    ANSEL_API * makeModel(RawModel* rawModel);
		static void  ANSEL_API destroy();

		static RawModel ANSEL_API * makeSquare();

		static RawModel* getRawModel(const unsigned int index);
		static void destroyRawModel (const unsigned int index);
	};
}
