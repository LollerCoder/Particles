#include "RenderLine.h"

void RenderLine::Update(MyVector p1, MyVector p2, glm::mat4 projectionMatrix)
{
	this->P1 = p1;
	this->P2 = p2;
	this->projectionMatrix = projectionMatrix;
}

void RenderLine::Draw(Shader* shader)
{
	//glUseProgram(shader->getID());
	shader->use();

	shader->setMat4("transform", glm::mat4(1.0f));

	glm::vec4 d1 = glm::vec4(
		P1.x, P1.y, P1.z, 1.0f
	);

	glm::vec4 d2 = glm::vec4(
		P2.x, P2.y, P2.z, 1.0f
	);

	glBegin(GL_LINES);
	glVertex3f(d1.x, d1.y, d1.z);
	glVertex3f(d2.x, d2.y, d2.z);
	glEnd();
}
