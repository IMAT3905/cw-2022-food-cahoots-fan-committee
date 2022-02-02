/** \file cameraTests.cpp */

#include "cameraTests.h"

TEST(Camera, CameraConstructor)
{
	Engine::CameraController camera;
	
	float speed = camera.getSpeed();
	float rotationSpeed = camera.getRotationSpeed();
	float sensitivity = camera.getSensitivity();
	glm::vec3 position = camera.getPosition();
	glm::vec2 mousePos = camera.getLastMousePos();
	glm::vec3 rotation = camera.getRotation();

	EXPECT_EQ(speed, 1.0f);
	EXPECT_EQ(rotationSpeed, 0.3f);
	EXPECT_EQ(sensitivity, 40.0f);
	EXPECT_EQ(position, glm::vec3(0.0, 0.0, 0.0));
	EXPECT_EQ(mousePos, Engine::InputPoller::getMousePosition());
	EXPECT_EQ(rotation, glm::vec3(0.0, 0.0, 0.0));
}