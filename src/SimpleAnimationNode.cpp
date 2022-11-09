#include "SimpleAnimationNode.h"

using namespace glm;

SimpleAnimationNode::SimpleAnimationNode(float rotationSpeed, vec3 rotationAxis)
    : rotationSpeed { rotationSpeed }, rotationAxis { rotationAxis }
{
}

void SimpleAnimationNode::updateNode(float dt, const mat4& model)
{
    // Update local transform by appending incremental rotation.
    this->localTransform = rotate(rotationSpeed * dt, rotationAxis) * this->localTransform;
}



SinAnimationNode::SinAnimationNode(float rotationSpeed, vec3 rotationAxis, float timerScale)
    : SimpleAnimationNode{ rotationSpeed, rotationAxis }, timerScale{ timerScale }
{

}

void SinAnimationNode::updateNode(float dt, const mat4& model)
{
    timer += dt * timerScale;
    // Update local transform by appending incremental rotation.
    this->localTransform = rotate(rotationSpeed * sin(timer), rotationAxis) * this->localTransform;
}
