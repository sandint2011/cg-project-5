#pragma once
#include "PointLight.h"

struct SpotLight : public PointLight
{
public:
    glm::vec3 direction = glm::vec3(0, -1, 0);
    float cutoff = 100.0f;

    // Override these from pointLight because mySPotLight.position and stuff doesn't work because C++ hates us.
    //glm::vec3 position = glm::vec3(-0.2, 1.5, 0);

    glm::vec3 position = glm::vec3(0,1,0);
    glm::vec3 color = glm::vec3(1, 0, 0);
    float intensity = 5;
    glm::vec3 getColorIntensity() const
    {
        return color * intensity;
    }
};
