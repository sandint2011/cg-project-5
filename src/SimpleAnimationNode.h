#pragma once
#include "SceneGraphNode.h"

class SimpleAnimationNode : public SceneGraphNode
{
public:
    SimpleAnimationNode(float rotationSpeed, glm::vec3 rotationAxis);

protected:
    virtual void updateNode(float dt, const glm::mat4& model) override;

    float rotationSpeed;
    glm::vec3 rotationAxis;
};

class SinAnimationNode : public SimpleAnimationNode
{
public:
    SinAnimationNode(float rotationSpeed, glm::vec3 rotationAxis, float timerScale);

protected:
    void updateNode(float dt, const glm::mat4& model) override;
    float timer = 0.0f;
    float timerScale = 1.0f;


};