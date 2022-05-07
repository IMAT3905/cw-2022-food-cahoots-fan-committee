/* \file sceneLayer.h */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <entt/entt.hpp>

#include "engine.h"
#include "include/platform/GLFW/GLFWCodes.h"
#include "rendering/renderer3D.h"
#include "rendering/renderer2D.h"
#include "components/relationship.h"
#include "components/transform.h"
#include "components/render.h"
#include "components/label.h"
#include "platform/OpenGL/OpenGLFrameBuffer.h"
#include "platform/GLFW/ImGuiHelper.h"

using namespace Engine;

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer(const char* name);
	void onRender() override;
	void onUpdate(float timestep) override;
	void onKeyPressed(KeyPressedEvent& e) override;
	void onResize(WindowResizeEvent& e) override;
	bool loadJSONfile();
	bool saveJSONfile();
private:
	SceneWideUniforms m_swu;
	glm::vec3 lightData[3] = { {1.0f, 1.0f, 1.0f}, { -2.0f, 4.0f, 6.0f }, { 0.0f, 0.0f, 0.0f } };

	std::shared_ptr<Material> pyramidMat;
	std::shared_ptr<Material> letterCubeMat;
	std::shared_ptr<Material> numberCubeMat;
	std::shared_ptr<Material> checkerCubeMat;
	std::shared_ptr<Material> conveyorMat;

	ImVec4 col = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);

	std::shared_ptr<Shader> TPShader;

	std::shared_ptr<VertexArray> cube, pyramid, geo;

	std::shared_ptr<CameraController> m_eulerCam; //!< An euler camera that can be moved and rotated
		
	entt::registry& m_registry;
	std::vector<entt::entity>& m_entities; 

	std::shared_ptr<FrameBuffer> m_sceneFBO;
	std::shared_ptr<FrameBuffer> m_defaultFBO;

	SceneWideUniforms m_swu2D;

	Quad m_screenQuad;
	SubTexture m_screenTexture;

	glm::mat4 view2D;
	glm::mat4 projection2D;

	bool m_mouseOverScene;
};
