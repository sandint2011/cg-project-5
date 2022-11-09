#pragma once

#include <vector>
#include "ofMain.h"
#include "Camera.h"
#include "SceneGraphNode.h"
#include "LitDrawNode.h"
#include "Lighting.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "SpotLightNode.h"

class ofApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

private:
	// Scene nodes...
	SceneGraphNode root;

	std::shared_ptr<SceneGraphNode> body;

	std::shared_ptr<SceneGraphNode> headAnimation;
	std::shared_ptr<SceneGraphNode> head;

	std::shared_ptr<SceneGraphNode> leftShoulder;
	std::shared_ptr<SceneGraphNode> leftLeg;
	std::shared_ptr<SceneGraphNode> leftFoot;

	std::shared_ptr<SceneGraphNode> rightShoulder;
	std::shared_ptr<SceneGraphNode> rightLeg;
	std::shared_ptr<SceneGraphNode> rightFoot;

	std::shared_ptr<SceneGraphNode> centerLeg;
	std::shared_ptr<SceneGraphNode> centerFoot;

	std::shared_ptr<SceneGraphNode> eyeBox;
	std::shared_ptr<SceneGraphNode> eye;

	std::shared_ptr<SceneGraphNode> flashlightJoint;
	std::shared_ptr<SceneGraphNode> flashlightAnimation;
	std::shared_ptr<SceneGraphNode> flashlight;

	// Meshes.
	ofMesh cylinderMesh;
	ofMesh sphereMesh;
	ofMesh coneMesh;
	ofMesh cubeMesh;

	// Lighting.
	DirectionalLight sceneDirectionalLight {glm::vec3(-1, -1, -1)};
	SpotLight sceneSpotLight {};
	PointLight scenePointLight {};
	Lighting sceneLighting{
		glm::vec3(0, 0, 0), // Ambient light.
		sceneDirectionalLight,
		sceneSpotLight,
		scenePointLight
	};

	ofShader shader;

	// Helper functions.
	float randf(float start, float end);

	// Professor's camera.
	Camera camera{ glm::vec3(2, 1.5, 2) };
	glm::vec3 velocity;
	int prevMouseX = ofGetViewportWidth() / 2;
	int prevMouseY = ofGetViewportHeight() / 2;
	float mouseSensitivity = 0.02f;
	float cameraHead = 0;
	float cameraPitch = 0;
	glm::vec3 cameraDirection = glm::vec3(0, 0, 0);
	glm::vec3 cameraFront = glm::vec3(0, 0, -1);
	glm::vec3 cameraUp = glm::vec3(0, 1, 0);
	void updateCameraRotation(float dx, float dy);

	// Shader reloading.
	bool needsShaderReload = true;
	void reloadShaders();
};
