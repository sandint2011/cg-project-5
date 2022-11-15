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
	// glEnable(GL_CULL_FACE); // Comment this out because the skybox won't draw then.

	// Load cube and invert the normals.
	cubeMesh.load("cube.ply");
	cubeMesh.flatNormals();
	for (int i = 0; i < cubeMesh.getNumNormals(); i++)
	{
		cubeMesh.setNormal(i, -cubeMesh.getNormal(i));
	}
	// Load sword and invert the normals.
	swordMesh.load("models/sword.ply");
	swordMesh.flatNormals();
	for (int i = 0; i < swordMesh.getNumNormals(); i++)
	{
		swordMesh.setNormal(i, -swordMesh.getNormal(i));
	}

	// Add animation to root.
	root.childNodes.emplace_back(new SinAnimationNode(0.01f, glm::vec3(0, 1, 0), 1.0f));
	root.childNodes.back()->localTransform = translate(vec3(0, 0, 0));
	animation = root.childNodes.back();

	// Add sword to animation.
	animation->childNodes.emplace_back(new LitDrawNode(swordMesh, shader, sceneLighting));
	animation->childNodes.back()->localTransform = rotate(radians(-30.0f), vec3(1, 0, 0));
	sword = animation->childNodes.back();

	// Skybox.
	cubemap.load("textures/skybox_front.png", "textures/skybox_back.png", "textures/skybox_right.png", "textures/skybox_left.png", "textures/skybox_top.png", "textures/skybox_bottom.png");

	reloadShaders();
}

//--------------------------------------------------------------
void ofApp::reloadShaders()
{
	if (needsShaderReload)
	{
		// Reload all shaders here.
		shader.load("shader.vert", "shader.frag");
		skyboxShader.load("skybox.vert", "skybox.frag");

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
	const float nearClip = 0.01;
	const float farClip = 2000;
	float aspectRatio = static_cast<float>(ofGetViewportWidth()) / static_cast<float>(ofGetViewportHeight());

	// Calculate view and projection matrices for camera.
	CameraMatrices cameraMatrices{ camera, aspectRatio, nearClip, farClip };

	// Draw the whole scene graph, starting with the root and recursively drawing its children.
	root.drawSceneGraph(cameraMatrices);

	// Draw skybox before objects (hopefully this works).
	//drawSkybox(cameraMatrices);
}

//--------------------------------------------------------------

void ofApp::drawSkybox(const CameraMatrices& cameraMatrices)
{
	using namespace glm;

	//mat4 model{ translate(cameraMatrices.getCamera().position) };
	
	skyboxShader.begin();
	glDepthFunc(GL_LEQUAL); // Pass depth test at far clipping plane.
	
	skyboxShader.setUniformMatrix4f("mvp", cameraMatrices.getProj() * mat4(mat3(cameraMatrices.getView())));
	skyboxShader.setUniformTexture("cubemap", cubemap.getTexture(), 0);
	cubeMesh.draw();

	glDepthFunc(GL_LESS); // Reset depth test.
	skyboxShader.end();
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