#include "PhysicsWorld.h"

using namespace P6;

void PhysicsWorld::AddParticle(P6Particle* toAdd) {
	Particles.push_back(toAdd);

	forceRegistry.Add(toAdd, &Gravity);
}
void PhysicsWorld::Update(float time) {
	UpdateParticleList();

	forceRegistry.UpdateForces(time);

	for (std::list<P6Particle*>::iterator p = Particles.begin(); p != Particles.end(); p++) {
		(*p)->Update(time);
	}
}

void PhysicsWorld::UpdateParticleList() {
	Particles.remove_if([](P6Particle* p) {
		return p->IsDestroyed();
	});
};