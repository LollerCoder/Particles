#pragma once

#include "Vector.h"
#include <iostream>

namespace P6 {

	
	class P6Particle {
	
		
	public://race
		std::string name = "Particle";
		bool finished = false;
		float placement = 0.0f;
		float magVel = 0.0f;
		MyVector avgVel = MyVector(0.0f, 0.0f, 0.0f);
		float time = 0.0f;
		MyVector initVel = MyVector(0.0f, 0.0f, 0.0f);
	public://fireworks
		float lifeSpan = 1.0f;
	public:
		float mass;
		MyVector Position;
		MyVector Velocity;
		MyVector Acceleration;
		
		
		//damping 
		float damping = 0.9f;
	public:
		P6Particle();
		P6Particle(float m, MyVector pos, MyVector vel, MyVector accel);
	protected:
		//forces
		MyVector accumulatedForce = MyVector(0, 0, 0);


		bool isDestroyed = false;
		void UpdateVelocity(float time);
		void UpdatePosition(float time);
		
	public:

		//forces
		void ResetForce();
		void AddForce(MyVector force);


		void Update(float time);
	public:
		void Destroy();
		bool IsDestroyed() {
			return isDestroyed;
		}
	};
}