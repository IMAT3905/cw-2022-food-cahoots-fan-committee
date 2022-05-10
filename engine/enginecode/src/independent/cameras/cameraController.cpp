/* \file cameraController.cpp */

#include "engine_pch.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "events/codes.h"
#include "cameras/cameraController.h"
#include "systems/log.h"
namespace Engine
{
    CameraController::CameraController(const CameraParams& e)
    {
        m_position = e.position;

        m_lastMousePos = InputPoller::getMousePosition();

        m_transform = glm::translate(glm::mat4(1.f), m_position);

        m_rotation = glm::vec3(-45.0f, 0.f, 0.f);

        m_camera.updateView(m_transform);

        m_camera.projection = glm::perspective(glm::radians(e.FOV), e.width / e.height, e.nearPlane, e.farPlane);

        m_params = e;
    }

    void CameraController::onUpdate(float timestep)
    {
        glm::vec3 right = { m_transform[0][0], m_transform[0][1], m_transform[0][2] };
        glm::vec3 forward = { -m_transform[2][0], -m_transform[2][1], -m_transform[2][2] };
        glm::vec3 up = { m_transform[1][0], m_transform[1][1], m_transform[1][0] };

        //strafing
        //if (InputPoller::isKeyPressed(NG_KEY_A)) m_position -= (right * m_speed * timestep);
        //if (InputPoller::isKeyPressed(NG_KEY_D)) m_position += (right * m_speed * timestep);

        //if (InputPoller::isKeyPressed(NG_KEY_W)) m_position += (forward * m_speed * timestep);
        //if (InputPoller::isKeyPressed(NG_KEY_S)) m_position -= (forward * m_speed * timestep);

        //rotation with key presses
        //if (InputPoller::isKeyPressed(NG_KEY_LEFT)) m_rotation.y += m_rotationSpeed * timestep;
        //if (InputPoller::isKeyPressed(NG_KEY_RIGHT)) m_rotation.y -= m_rotationSpeed * timestep;

        //if (InputPoller::isKeyPressed(NG_KEY_UP)) m_rotation.x += m_rotationSpeed * timestep;
        //if (InputPoller::isKeyPressed(NG_KEY_DOWN)) m_rotation.x -= m_rotationSpeed * timestep;

        //Mouse controls for camera
        glm::vec2 currentMousePos = InputPoller::getMousePosition();
        glm::vec2 deltaMousePos = currentMousePos - m_lastMousePos;
        deltaMousePos /= glm::vec2(1024, 800);

        m_lastMousePos = currentMousePos;

        //m_rotation.y -= m_sensitivity * deltaMousePos.x * timestep;
        //m_rotation.x -= m_sensitivity * deltaMousePos.y * timestep;

        glm::mat4 rotationY = glm::rotate(glm::mat4(1.f), m_rotation.y, glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 rotationX = glm::rotate(glm::mat4(1.f), m_rotation.x, glm::vec3(1.f, 0.f, 0.f));

        glm::mat4 rotation = rotationX * rotationY;

        m_transform = glm::translate(glm::mat4(1.f), m_position) * rotation;

        m_camera.updateView(m_transform);
    }

    void CameraController::onResize(WindowResizeEvent& e)
    {
        m_params.width = e.getWidth();
        m_params.height = e.getHeight();

        m_camera.projection = glm::perspective(glm::radians(m_params.FOV), m_params.width / m_params.height, m_params.nearPlane, m_params.farPlane);
    }
}