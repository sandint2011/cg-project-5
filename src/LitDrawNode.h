#pragma once
#include "SimpleDrawNode.h"
#include "Lighting.h"

class LitDrawNode : public SimpleDrawNode
{
public:
    // Constructor
    LitDrawNode(const ofMesh& mesh, const ofShader& shader, const Lighting& sceneLighting);

    // Override the base class's draw function
    void drawNode(const CameraMatrices& camera, const glm::mat4& model) const;

    glm::vec3 meshColor { glm::vec3(0.1, 0.6, 1.0) }; // Light blue.
    const Lighting& sceneLighting;
};