#pragma once

#include "../Def.h"
#include "../util.h"

#include "Model.h"
#include "camera.h"

namespace Ansel
{
	namespace PSys {
		struct ParticleProperties
		{
			int rate = 10;
			float gravity = 0.1f;
			float life = 2.f;
			float initial_velocity = 2.f;

			vec4f birth_color = { 1, 1, 1, 1 };
			vec4f death_color = { 1, 1, 1, 1 };
		};

		struct Particle
		{
			vec4f c_color, start_color, end_color;
			vec3f location, velocity;
			float t_create, t_life, t_limit;

			unsigned int frame_life = 0;
		};
	}

	class ParticleSystem
	{
		std::vector<PSys::Particle*> particles;

		RawModel* particleModel;
		PSys::ParticleProperties properties;

		static vec4f getGradient(vec4f colora, vec4f colorb, float k);

		void makeParticle();

		int type;
		
		vec3f _location, _lastlocation, _velocity;
		FloatRect _plane;
		float _height;

		bool baked = false;

	public:

		enum EMITTER_TYPE {
			POINT,
			PLANE,
			BOX
		};

		ANSEL_API ParticleSystem(vec3f location);

		void ANSEL_API update();
		void ANSEL_API set();

		PSys::ParticleProperties ANSEL_API * getProperties();
		void               ANSEL_API   setProperties(PSys::ParticleProperties p);

		void ANSEL_API setEmitterType(int t);
		void ANSEL_API setPlane(FloatRect plane);
		void ANSEL_API setHeight(float height);

		void  ANSEL_API addLocation(vec3f loc);
		void  ANSEL_API setLocation(vec3f loc);
		vec3f ANSEL_API getLocation() const;

		RawModel ANSEL_API * getModel() const;
		PSys::Particle ANSEL_API * getParticle(int index) const;

		unsigned int ANSEL_API getParticleSize() const;
	};
}