#include "Particle.h"

using namespace P6;

P6Particle::P6Particle() : Position(0,0,0), Velocity(0, 0, 0), Acceleration(0, 0, 0), mass(1) {

}
void P6Particle::UpdateVelocity(float time) {
	this->Velocity = this->Velocity + this->Acceleration * time;
	
}
void P6Particle::UpdatePosition(float time) {
	this->Position = this->Position + this->Velocity * time +( this->Acceleration * time * time / 2);
}

void P6Particle::Update(float time) {
	this->UpdatePosition(time);
	this->UpdateVelocity(time);
}