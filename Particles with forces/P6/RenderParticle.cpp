#include "RenderParticle.h"

void RenderParticle::Draw() {
	if (!PhysicsParticle->IsDestroyed()) {
		//set color
		RenderObject->setColor(this->Color.x, this->Color.y, this->Color.z);
		//set pos
		RenderObject->setPos(this->PhysicsParticle->Position.x, this->PhysicsParticle->Position.y, this->PhysicsParticle->Position.z);

		RenderObject->draw(this->shaderProg, this->VAO, this->fullVertexData);

	}
}