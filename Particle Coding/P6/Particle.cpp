#include "Particle.h"


using namespace P6;

P6Particle::P6Particle() : Position(0.f,0.f,0.f), Velocity(0.f, 0.f, 0.f), Acceleration(0.f, 0.f, 0.f), mass(1.f) {

}

P6::P6Particle::P6Particle(float m, MyVector pos, MyVector vel, MyVector accel)
{
	this->mass = m;
	this->Position = pos;
	this->Velocity = vel;
	this->Acceleration = accel;
}

void P6Particle::UpdateVelocity(float time) {
	this->Velocity += this->Acceleration * time;
	
}

void P6Particle::UpdatePosition(float time) {
	this->Position += (this->Velocity * time) + (this->Acceleration * time * time) * (1.0f / 2.0f);
}

void P6Particle::Update(float time) {
	this->UpdatePosition(time);
	this->UpdateVelocity(time);
}
