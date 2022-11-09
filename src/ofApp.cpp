#include "ofApp.h"
#include <vector>
#include <random>
#include "CameraMatrices.h"
#include "SimpleDrawNode.h"
#include "SimpleAnimationNode.h"
#include "LitDrawNode.h"
#include "Lighting.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "SpotLightNode.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	using namespace glm;

	ofDisableArbTex();

	ofEnableDepthTest();
	glEnable(GL_CULL_FACE);

	//ofSetBackgroundColor(135, 205, 235, 255);

	// Load cylinder and invert the normals.
	cylinderMesh.load("cylinder.ply");
	cylinderMesh.flatNormals();
	for (int i = 0; i < cylinderMesh.getNumNormals(); i++)
	{
		cylinderMesh.setNormal(i, -cylinderMesh.getNormal(i));
	}
	// Load sphere and invert the normals.
	sphereMesh.load("sphere.ply");
	sphereMesh.flatNormals();
	for (int i = 0; i < sphereMesh.getNumNormals(); i++)
	{
		sphereMesh.setNormal(i, -sphereMesh.getNormal(i));
	}
	// Load cone and invert the normals.
	coneMesh.load("cone.ply");
	coneMesh.flatNormals();
	for (int i = 0; i < coneMesh.getNumNormals(); i++)
	{
		coneMesh.setNormal(i, -coneMesh.getNormal(i));
	}
	// Load cube and invert the normals.
	cubeMesh.load("cube.ply");
	cubeMesh.flatNormals();
	for (int i = 0; i < cubeMesh.getNumNormals(); i++)
	{
		cubeMesh.setNormal(i, -cubeMesh.getNormal(i));
	}

	// Initialize scene lighting.
	sceneSpotLight.position = glm::vec3(0, 0, 0);
	sceneSpotLight.direction = glm::vec3(1, -1, -1);
	scenePointLight.position = glm::vec3(0, 0, 0);

	// Add body to root.
	root.childNodes.emplace_back(new LitDrawNode(cylinderMesh, shader, sceneLighting));
	root.childNodes.back()->localTransform = rotate(radians(-20.0f), vec3(1, 0, 0));
	body = root.childNodes.back();

	// Add head animation to body.
	body->childNodes.emplace_back(new SinAnimationNode(0.01f, glm::vec3(0, 1, 0), 1.0f));
	body->childNodes.back()->localTransform = translate(vec3(0, 1, 0));
	headAnimation = body->childNodes.back();

	// Add head to head animation.
	headAnimation->childNodes.emplace_back(new SimpleDrawNode(sphereMesh, shader));
	headAnimation->childNodes.back()->localTransform = translate(vec3(0,0,0));
	head = headAnimation->childNodes.back();

	// Add left shoulder to body.
	body->childNodes.emplace_back(new SimpleDrawNode(cylinderMesh, shader));
	body->childNodes.back()->localTransform = translate(vec3(-1.1, 0.5, 0)) * rotate(radians(40.0f), vec3(1, 0, 0)) * rotate(radians(90.0f), vec3(0, 0, 1)) * scale(vec3(.35, .1, .35));
	leftShoulder = body->childNodes.back();

	// Add right shoulder to body.
	body->childNodes.emplace_back(new SimpleDrawNode(cylinderMesh, shader));
	body->childNodes.back()->localTransform = translate(vec3(1.1, 0.5, 0)) * rotate(radians(40.0f), vec3(1, 0, 0)) * rotate(radians(-90.0f), vec3(0, 0, 1)) * scale(vec3(.35, .1, .35));
	rightShoulder = body->childNodes.back();

	// Add left leg to left shoulder.
	leftShoulder->childNodes.emplace_back(new SimpleDrawNode(cubeMesh, shader));
	leftShoulder->childNodes.back()->localTransform = translate(vec3(-3, 0, 0)) * scale(vec3(2.75, 1, 0.75));
	leftLeg = leftShoulder->childNodes.back();

	// Add right leg to right shoulder.
	rightShoulder->childNodes.emplace_back(new SimpleDrawNode(cubeMesh, shader));
	rightShoulder->childNodes.back()->localTransform = translate(vec3(3, 0, 0)) * scale(vec3(2.75, 1, 0.75));
	rightLeg = rightShoulder->childNodes.back();

	// Add center leg to body.
	body->childNodes.emplace_back(new SimpleDrawNode(cubeMesh, shader));
	body->childNodes.back()->localTransform = translate(vec3(0, -1.25, 0)) * scale(vec3(0.125, 0.5, 0.25));
	centerLeg = body->childNodes.back();

	// Add left foot to left arm.
	leftLeg->childNodes.emplace_back(new SimpleDrawNode(coneMesh, shader));
	leftLeg->childNodes.back()->localTransform = translate(vec3(-0.9, 0, 0)) * rotate(radians(5.0f), vec3(0, 1, 0)) * rotate(radians(-90.0f), vec3(0, 0, 1)) * scale(vec3(4, 0.5, 2));
	leftFoot = leftLeg->childNodes.back();

	// Add right foot to right arm.
	rightLeg->childNodes.emplace_back(new SimpleDrawNode(coneMesh, shader));
	rightLeg->childNodes.back()->localTransform = translate(vec3(0.9, 0, 0)) * rotate(radians(-5.0f), vec3(0, 1, 0)) * rotate(radians(90.0f), vec3(0, 0, 1)) * scale(vec3(4, 0.5, 2));
	rightFoot = rightLeg->childNodes.back();

	// Add center foot to center leg.
	centerLeg->childNodes.emplace_back(new SimpleDrawNode(coneMesh, shader));
	centerLeg->childNodes.back()->localTransform = translate(vec3(0, -0.9, 0)) * rotate(radians(10.0f), vec3(1, 0, 0)) * scale(vec3(4, 0.5, 2));
	centerFoot = centerLeg->childNodes.back();

	// Add eye box to head.
	head->childNodes.emplace_back(new SimpleDrawNode(cubeMesh, shader));
	head->childNodes.back()->localTransform = translate(vec3(0, 0.55, 0.65)) * rotate(radians(-35.0f), vec3(1, 0, 0)) * scale(vec3(0.15, 0.15, 0.15));
	eyeBox = head->childNodes.back();

	// Add eye to eye box.
	eyeBox->childNodes.emplace_back(new SimpleDrawNode(sphereMesh, shader));
	eyeBox->childNodes.back()->localTransform = translate(vec3(0, 0, 0.5)) * scale(vec3(0.9, 0.9, 0.9));
	eye = eyeBox->childNodes.back();

	// Add flaghlight joint to head.
	head->childNodes.emplace_back(new SimpleDrawNode(sphereMesh, shader));
	head->childNodes.back()->localTransform = translate(vec3(0.25, 0.2, 0.85)) * rotate(radians(-35.0f), vec3(1, 0, 0)) * scale(vec3(0.1, 0.1, 0.1));
	flashlightJoint = head->childNodes.back();

	// Add flashlight animation to flashlight joint.
	flashlightJoint->childNodes.emplace_back(new SinAnimationNode(0.02f, vec3(.3,.3,.3), 3.23f));
	flashlightJoint->childNodes.back()->localTransform = rotate(radians(30.0f), vec3(1, 0, 0));
	flashlightAnimation = flashlightJoint->childNodes.back();

	// Add flashlight to flashlight animation.
	flashlightAnimation->childNodes.emplace_back(new SimpleDrawNode(cylinderMesh, shader));
	flashlightAnimation->childNodes.back()->localTransform = translate(vec3(0, 0, 0.75)) * rotate(radians(90.0f), vec3(1, 0, 0)) * scale(vec3(0.6, 0.6, 0.6));
	flashlight = flashlightAnimation->childNodes.back();

	reloadShaders();
}

//--------------------------------------------------------------
void ofApp::reloadShaders()
{
	if (needsShaderReload)
	{
		// Reload all shaders here.
		shader.load("shader.vert", "shader.frag");

		needsShaderReload = false;
	}
}

//--------------------------------------------------------------
void ofApp::update()
{
	reloadShaders();

	using namespace glm;

	camera.position += mat3(rotate(cameraHead, vec3(0, 1, 0))) * velocity * ofGetLastFrameTime();
	camera.rotation = (rotate(cameraHead, vec3(0, 1, 0)) * rotate(cameraPitch, vec3(1, 0, 0)));

	root.updateSceneGraph(ofGetLastFrameTime(),glm::mat4());
}

//--------------------------------------------------------------
void ofApp::draw()
{
	using namespace glm;
	
	// Camera settings.
	const float nearClip = 0.1;
	const float farClip = 200;
	float aspectRatio = static_cast<float>(ofGetViewportWidth()) / static_cast<float>(ofGetViewportHeight());

	// Calculate view and projection matrices for camera.
	CameraMatrices cameraMatrices{ camera, aspectRatio, nearClip, farClip };

	// Draw the whole scene graph, starting with the root and recursively drawing its children.
	root.drawSceneGraph(cameraMatrices);
}

//--------------------------------------------------------------
void ofApp::exit()
{
	
}

//--------------------------------------------------------------
void ofApp::updateCameraRotation(float dx, float dy)
{
	using namespace glm;
	cameraPitch -= dy;
	cameraPitch = clamp(cameraPitch, radians(-89.0f), radians(89.0f));
	cameraHead -= dx;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	using namespace glm;

	if (key == '`')
	{
		needsShaderReload = true;
	}

	const float cameraSpeed = 20;
	const float sprint = 5;
	const float dt = ofGetLastFrameTime();

	//// Forward / backward.
	//if (key == 'w')
	//	camera.position += cameraFront * cameraSpeed * dt;
	//else if (key == 'W')
	//	camera.position += cameraFront * cameraSpeed * sprint * dt;
	//if (key == 's')
	//	camera.position -= cameraFront * cameraSpeed * dt;
	//else if (key == 'S')
	//	camera.position -= cameraFront * cameraSpeed * sprint * dt;

	//// Left / right.
	//if (key == 'a')
	//	camera.position -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * dt;
	//else if (key == 'A')
	//	camera.position -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * sprint * dt;
	//if (key == 'd')
	//	camera.position += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * dt;
	//else if (key == 'D')
	//	camera.position += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * sprint * dt;

	//// Up / down.
	//if (key == 'q')
	//	camera.position += cameraUp * cameraSpeed * dt;
	//else if (key == 'Q')
	//	camera.position += cameraUp * cameraSpeed * sprint * dt;
	//if (key == 'e')
	//	camera.position -= cameraUp * cameraSpeed * dt;
	//else if (key == 'E')
	//	camera.position -= cameraUp * cameraSpeed * sprint * dt;

	// Move along the x axis.
	if (key == 'x')
		camera.position.x += cameraSpeed * dt;
	else if (key == 'X')
		camera.position.x -= cameraSpeed * dt;

	// Move along the y axis.
	if (key == 'y')
		camera.position.y += cameraSpeed * dt;
	else if (key == 'Y')
		camera.position.y -= cameraSpeed * dt;

	// Move along z axis.
	if (key == 'z')
		camera.position.z += cameraSpeed * dt;
	else if (key == 'Z')
		camera.position.z -= cameraSpeed * dt;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
	// Get the mouse's change ins position since last frame.
	float dx = (x - prevMouseX);
	float dy = (y - prevMouseY);
	prevMouseX = x;
	prevMouseY = y;

	// Calculate camera direction.
	cameraDirection;
	cameraDirection.x = cos(cameraHead - glm::radians(90.0f)) * cos(cameraPitch); // Sutract 90 degrees from head because otherwise 0 radians points at +X instead of -Z like we want.
	cameraDirection.y = -sin(cameraPitch); // Negative because otherwise we're doing inverted Y and that's gross.
	cameraDirection.z = sin(cameraHead - glm::radians(90.0f)) * cos(cameraPitch); // Sutract 90 degrees from head because otherwise 0 radians points at +X instead of -Z like we want.
	cameraFront = glm::normalize(cameraDirection);

	updateCameraRotation(dx * mouseSensitivity, dy * mouseSensitivity);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

//--------------------------------------------------------------
float ofApp::randf(float start = 0.0, float end = 1.0)
{
	float r = ((float)rand()) / (float)RAND_MAX;
	r = r * (end - start) + start;
	return r;
}