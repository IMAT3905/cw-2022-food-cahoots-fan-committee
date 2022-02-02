/* \file camera.h */

#pragma once

#include <glm/glm.hpp>

namespace Engine
{
    /** \class Camera
    ** \brief A class which will hold data related to the camera
    */
    class Camera
    {
    public:
        glm::mat4 view; //!< The view matrix
        glm::mat4 projection; //!< The projection matrix
        void updateView(const glm::mat4& transform) { view = glm::inverse(transform); } //!< A function used to regularly update the viewpoint of the camera \param transform which is any transformation that will be applied to the camera
    };
}
