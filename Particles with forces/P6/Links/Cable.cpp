/*
    This will make it so that the ball is always x lenght from the anchorpoint. Velocity is maintained by reducing it with displacement
    direction multiplied to velocity
*/

#include "Cable.h"

void P6::Cable::UpdateForce(P6Particle* particle, float time)
{
    MyVector pos = particle->Position;

    //displacement
    MyVector displacement = pos - this->anchorPoint;
    // use to check 
    float mag = displacement.Magnitude();

    if (mag > this->length) {
        //just set the position to max length if its violating it.
        MyVector direction = displacement.Direction();
        pos = this->anchorPoint + direction * this->length;
        particle->Position = pos;

       //velocity that will be screwed with
        MyVector velocity = particle->Velocity;

        //apply normal force to the obj
        MyVector normalForce = direction * velocity.DotProduct(direction);

        MyVector backVelocity = velocity - normalForce;
        
        particle->Velocity = backVelocity;
    }
    //add force is gone because all we did was add normal force.
    
    
}
