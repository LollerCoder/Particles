#pragma once

#include <list>
#include "Particle.h"

namespace P6 {
	class PhysicsWorld {
	public:
		std::list<P6Particle*> Particles;

		void AddParticle(P6Particle* toAdd);
		void Update(float time);

	private:
		void UpdateParticleList();
	};
}