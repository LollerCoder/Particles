/*

*/

#include "Cable.h"

void P6::Cable::UpdateForce(P6Particle* particle, float time)
{
	this->currLength = (this->anchorPoint - particle->Position).Magnitude();
	if (this->currLength < this->length || particle->Position.y >= 0) return;

	MyVector force;

	if (this->currLength > this->length && particle->Position.y >= 0)
	{
		std::cout << "Here";
		MyVector direction = this->anchorPoint - particle->Position;
		direction *= particle->Velocity;

		force = direction * particle->mass;
	}
	else
	{
		std::cout << "Here2";
		force = (MyVector(0, 9.8f, 0) + particle->Velocity * -1) * particle->mass;
	}

	particle->AddForce(force);
}
