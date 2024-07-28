#pragma once

#include "ParticleLink.h"

#include "../ForceGenerator.h"
#include "../Vector.h"

namespace P6 {
	class Cable : public ForceGenerator {
	public:
		float length;
		float currLength = 0;
		float restitution = 0.9;
		MyVector anchorPoint;

	public:
		Cable(MyVector ap, float l) : anchorPoint(ap), length(l) {}

		void UpdateForce(P6Particle* p, float time) override;
	};
}