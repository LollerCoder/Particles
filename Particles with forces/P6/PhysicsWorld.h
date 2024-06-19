#pragma once

#include <list>
#include "Particle.h"
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"



namespace P6 {
	class PhysicsWorld {
	public:
		ForceRegistry forceRegistry;

	public:
		std::list<P6Particle*> Particles;

		void AddParticle(P6Particle* toAdd);

		void Update(float time);

	private:
		//checking what particles need to be removed
		void UpdateParticleList();

		GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, -9.8, 0));
	};
}

