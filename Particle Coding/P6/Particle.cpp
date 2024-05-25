#include "Particle.h"


using namespace P6;

P6Particle::P6Particle() : Position(0.f,0.f,0.f), Velocity(0.f, 0.f, 0.f), Acceleration(0.f, 0.f, 0.f), mass(1.f) {

}
void P6Particle::UpdateVelocity(float time) {
	this->Velocity = this->Velocity + this->Acceleration * time;
	
}
void P6Particle::UpdatePosition(float time) {
	this->Position = this->Position + this->Velocity * time +( this->Acceleration * time * time / 2);
	
	if (this->Position.y > -300) {
		std::cout << "Y:" << this->Position.y << std::endl;

	}
	
	
}

void P6Particle::Update(float time) {
	this->UpdatePosition(time);
	this->UpdateVelocity(time);
}