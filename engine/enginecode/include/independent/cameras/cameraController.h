/* \file cameraController.h */
#pragma once

#include "cameras/Camera.h"
#include "core/inputPoller.h"
#include "events/events.h"

namespace Engine
{
    struct CameraParams
    {
    public:
        glm::vec3 position = glm::vec3(0.f);
        float FOV = 45.f;
        float height;
        float width;
        float nearPlane = 0.1f;
        float farPlane = 100.f;
    };

    /** \class CameraController
    ** \brief A class which is used to create a controllable camera that can use WASD to move and the mouse/arrow keys to rotate
    */
    class CameraController
    {
    public:
        CameraController(const CameraParams& e); //!< Constructor that places the camera's position to 0, 0, 0
        Camera& getCamera() { return m_camera; } //!< Returns the camera being used
        virtual void onUpdate(float timestep); //!< A function to update the camera's position and rotation every frame
        virtual void onResize(WindowResizeEvent& e);
        virtual glm::vec3 getPosition() { return m_position; }
        virtual float getSpeed() { return m_speed; }
        virtual glm::vec2 getLastMousePos() { return m_lastMousePos; }
        virtual glm::vec3 getRotation() { return m_rotation; }
        virtual float getRotationSpeed() { return m_rotationSpeed; }
        virtual float getSensitivity() { return m_sensitivity; }
    private:
        Camera m_camera; //!< The camera object
        glm::mat4 m_transform; //!< Camera's transformation matrix
        glm::vec3 m_position; //!< Camera's current position
        float m_speed = 1.f; //!< Speed for movement using WASD
        glm::vec2 m_lastMousePos; //!< Last position of the mouse
        glm::vec3 m_rotation; //!< Camera's current rotation
        float m_rotationSpeed = 0.3f; //!< Speed for rotating using the arrow keys
        float m_sensitivity = 40.0f; //!< Sensitivity of the mouse movement for rotating the camera
        CameraParams m_params;
    };
}