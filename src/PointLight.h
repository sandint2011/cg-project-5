#pragma once
#include "ofMain.h"

struct PointLight
{
public:
    // Hardcode these because myPointLight.position = someVector doesn't work because C++ hates us.
    glm::vec3 position = glm::vec3(-0.05, 0, 1);
    glm::vec3 color { 1, 0.5, 0 };
    float cutoff = 100.0f;
    float intensity { 0.5 };

    glm::vec3 getColorIntensity() const
    {
        return color * intensity;
    }
};
