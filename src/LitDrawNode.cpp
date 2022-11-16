#include "LitDrawNode.h"

LitDrawNode::LitDrawNode(const ofMesh& mesh, const ofShader& shader, const Lighting& sceneLighting, ofImage colorTexture, ofImage metallicTexture, ofImage normalTexture)
    : SimpleDrawNode(mesh, shader), sceneLighting { sceneLighting }
{
    this->colorTexture = colorTexture;
    this->metallicTexture = metallicTexture;
    this->normalTexture = normalTexture;
}

void LitDrawNode::drawNode(const CameraMatrices& camera, const glm::mat4& model) const
{
    using namespace glm;

    mat4 mvp { camera.getProj() * camera.getView() * model };

    shader.begin(); // start using the shader
    shader.setUniformMatrix4f("mvp", mvp);
    shader.setUniformMatrix4f("model", model);
    shader.setUniformMatrix3f("normalMatrix", transpose(inverse(model)));

    //shader.setUnivorm2f("uv", );
    shader.setUniformTexture("colorTexture", colorTexture.getTexture(), 0);
    //shader.setUniformTexture("metallicTexture", metallicTexture.getTexture());
    //shader.setUniformTexture("normalTexture", normalTexture.getTexture());

    shader.setUniform3f("meshColor", meshColor);
    shader.setUniform3f("ambientColor", sceneLighting.ambientLight);

    shader.setUniform3f("dirLightColor", sceneLighting.dirLight.getColorIntensity());
    shader.setUniform3f("dirLightDir", sceneLighting.dirLight.getDirectionToLight());

    shader.setUniform3f("spotLightColor", sceneLighting.spotLight.getColorIntensity());
    shader.setUniform3f("spotLightConeDir", sceneLighting.spotLight.direction);
    shader.setUniform3f("spotLightPos", sceneLighting.spotLight.position);
    shader.setUniform1f("spotLightCutoff", sceneLighting.spotLight.cutoff);
    shader.setUniform1f("spotLightIntensity", sceneLighting.spotLight.intensity);

    // shader.setUniform3f("pointLightColor", sceneLighting.pointLight.getColorIntensity());
    // shader.setUniform3f("pointLightPos", sceneLighting.pointLight.position);
    // shader.setUniform1f("pointLightCutoff", sceneLighting.pointLight.cutoff);
    // shader.setUniform1f("pointLightIntensity", sceneLighting.pointLight.intensity);
    mesh.draw(); // draw mesh
    shader.end(); // done with the shader
}
