#pragma once

#include "Def.h"
#include "util.h"

#include <fstream>

#include <stdexcept>
#include <vector>
#include <algorithm>

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
			INDICES = 16
		} typedef BUFFER_TYPE;

		GLuint ID = 0;
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

		static GLenum getType(Buffer::BUFFER_TYPE type);

		void genBuffer(const Buffer::BUFFER_TYPE type);
		void bindBufferData(const std::vector<vec2f> data, Buffer::BUFFER_TYPE type = Buffer::BUFFER_TYPE::VERTEX);
		void bindBufferData(const std::vector<vec3f> data, Buffer::BUFFER_TYPE type = Buffer::BUFFER_TYPE::VERTEX);
		void bindBufferData(const std::vector<vec4f> data, Buffer::BUFFER_TYPE type = Buffer::BUFFER_TYPE::VERTEX);
		void bindBufferData(const std::vector<int> data, 
			const unsigned int size = 2, const Buffer::BUFFER_TYPE type = Buffer::BUFFER_TYPE::VERTEX);
		void bindBufferData(const std::vector<unsigned int> data,
			const unsigned int size = 2, const Buffer::BUFFER_TYPE type = Buffer::BUFFER_TYPE::VERTEX);

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
		VAO *vao;			///< Pointer to the VAO object that defines this model
		unsigned int size;	///< Amount of vertices/indices
		unsigned int index;	///< Respective index inside the vector of RawModels
		bool use_colors = false;
		bool use_normals = false;

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

		int colorsOn() const;
		int normalsOn() const;

		ANSEL_API ~RawModel();
	};

	/**
	  * Contains only a pointer to its corresponding RawModel and transformation data.
	  * All models of a specific RawModel are grouped together and rendered at once
	  * with all transformation data being uploaded to the VAO every frame.
	  */
	class Model
	{
		vec4f _location;	///< (x, y, z, w) location.
		vec3f _scale;		///< (x, y, z) scale factor.
		vec3f _rotation;	///< Pitch, yaw, and roll.

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

		void ANSEL_API setScale(vec3f scale);

		vec4f	  ANSEL_API getLocation( )   const;
		vec3f	  ANSEL_API getScale( )      const;
		vec3f	  ANSEL_API getRotation( )   const;
		RawModel  ANSEL_API * getRawModel( ) const;
	};

	class Loader
	{
		static std::vector<VAO*> vaos;
		static std::vector<RawModel*> rawModels;
		static std::vector<std::string> splitString(const char delimiter, std::string str);

		static int createVAO();

		static unsigned int nextModelIndex;

	public:

		static RawModel ANSEL_API * makeRawModel(std::vector<vec2f> vertices);
		static RawModel ANSEL_API * makeRawModel(std::vector<vec3f> vertices);
		static RawModel ANSEL_API * makeRawModel(std::vector<vec2f> vertices, std::vector<unsigned int> indices);
		static RawModel ANSEL_API * makeRawModel(std::vector<vec3f> vertices, std::vector<unsigned int> indices);
		static RawModel ANSEL_API * makeRawModel(std::vector<vec3f> vertices, std::vector<unsigned int> indices, std::vector<vec3f> normals);
		static RawModel ANSEL_API * makeRawModel(const char* filename);
		static Model    ANSEL_API * makeModel(RawModel* rawModel);
		static void  ANSEL_API destroy();

		static RawModel* getRawModel(const unsigned int index);
		static void destroyRawModel (const unsigned int index);
	};
}
