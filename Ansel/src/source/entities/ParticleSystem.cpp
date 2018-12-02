#include "../../headers/entities/ParticleSystem.h"

#include "../../headers/Engine.h"

#include <ECS.h>
#include <AnselAlloc.h>

namespace Ansel
{
	vec4f ParticleSystem::getGradient(vec4f colora, vec4f colorb, float k) {
		if (k > 1.f || k < 0.f) return { -1, -1, -1, -1 };

		const float a_amount = 1.f - k;
		const float b_amount = k;

		colora *= a_amount;
		colorb *= b_amount;

		return colora + colorb;
	}

	ParticleSystem::ParticleSystem(vec3f location) : _location(location) {
		particleModel = Loader::makeRawModel("assets/models/cube.obj");

		std::vector<vec4f> colors;
		for (int i = 0; i < particleModel->getVertexCount(); i++) {
			colors.push_back({ 0, 1, 0, 1 });
		}

		_velocity     = { 0, 0, 0 };
		_lastlocation = _location;
		particleModel->loadColors(colors);
	}

	void ParticleSystem::makeParticle() {
		PSys::Particle* part = new PSys::Particle;

		switch (type)
		{
		case POINT:
			part->location = _location; break;

		case PLANE:
		{
			const float x = (float)(rand() % 1000) / 1000.f * _plane.w;
			const float z = (float)(rand() % 1000) / 1000.f * _plane.h;

			part->location = {
				x - (_plane.w / 2) + _location.x,
				_location.y,
				z - (_plane.h / 2) + _location.z
			};
			break;
		}

		case BOX:
		{
			const float x = (float)(rand() % 1000) / 1000.f * _plane.w;
			const float y = (float)(rand() % 1000) / 1000.f * _height;
			const float z = (float)(rand() % 1000) / 1000.f * _plane.h;

			part->location = {
				x - (_plane.w / 2) + _location.x,
				y - (_height / 2.f) + _location.y,
				z - (_plane.h / 2) + _location.z
			};
			break;
		}

		default:
			break;
		}
		
		part->velocity = { 
			((rand() % 10) - 5) / 5.f * properties.initial_velocity, 
			((rand() % 10) - 5) / 5.f * properties.initial_velocity, 
			((rand() % 10) - 5) / 5.f * properties.initial_velocity
		};

		part->velocity += _velocity  / 4.f ;

		part->t_create = Engine::getTime();
		part->t_life = Engine::getTime() - part->t_create;
		part->c_color = { 1, 1, 1, 1 };

		particles.push_back(part);
	}

	void ParticleSystem::update() {
		if (!baked) return;

		_velocity = _location - _lastlocation;

		for (int i = 0; i < rand() % (int)std::ceil((float)properties.rate / 5.f); i++) {
			if (particles.size() + i + 1 > properties.rate * 10 * properties.life) continue;

			makeParticle();
		}

		unsigned int _i = 0;
		for (PSys::Particle* p : particles) {
			p->location   += p->velocity;
			p->t_life      = Engine::getTime() - p->t_create;
			p->location.y -= properties.gravity * p->t_life;
			
			p->c_color = getGradient(properties.birth_color, properties.death_color, p->t_life / properties.life);

			if (p->t_life > properties.life) {
				particles.erase(particles.begin() + _i);
				continue;
			}

			p->frame_life++;
		}

		_lastlocation = _location;
	}

	void ParticleSystem::set() {
		// When we set the system, we allocate all the space for the maximum amount of 
		// particles
		Alloc::IndexedAllocator<PSys::Particle>::makeNew(properties.rate * 10 * properties.life);

		baked = true;
	}

	PSys::ParticleProperties* ParticleSystem::getProperties() {
		return &properties;
	}

	void ParticleSystem::setProperties(PSys::ParticleProperties p) {
		properties = p;
	}

	void ParticleSystem::setHeight(float height) {
		_height = height;
	}

	void ParticleSystem::setEmitterType(int t) {
		type = t;
	}

	void ParticleSystem::setPlane(FloatRect plane) {
		_plane = plane;
	}

	void ParticleSystem::addLocation(vec3f loc) {
		_location += loc;
	}

	void ParticleSystem::setLocation(vec3f loc) {
		_location = loc;
	}

	vec3f ParticleSystem::getLocation() const {
		return _location;
	}

	RawModel* ParticleSystem::getModel() const {
		return particleModel;
	}

	PSys::Particle* ParticleSystem::getParticle(int index) const {
		return particles.at(index);
	}

	unsigned int ParticleSystem::getParticleSize() const {
		return particles.size();
	}
}