#pragma once

#include "Vector.h"
#include <iostream>

namespace P6 {

	
	class P6Particle {
	public:
		float mass;
		MyVector Position;
		MyVector Velocity;
		MyVector Acceleration;
	public:
		P6Particle();
		P6Particle(float m, MyVector pos, MyVector vel, MyVector accel);
	public:
		void UpdateVelocity(float time);
		void UpdatePosition(float time);
		
	public:
		void Update(float time);
	};
}