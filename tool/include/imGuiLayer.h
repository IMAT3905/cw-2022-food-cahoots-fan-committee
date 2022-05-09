/* \file imGuiLayer.h */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <entt/entt.hpp>

#include "engine.h"
#include "include/platform/GLFW/GLFWCodes.h"
#include "rendering/renderer3D.h"
#include "rendering/renderer2D.h"
#include "rendering/Renderer2DBillboard.h"
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
	//glm::vec3 lightData[3] = { {1.f, 1.f, 1.f}, {1.0f, 40.0f, 6.0f}, {0.0f, 0.0f, 0.0f} };

	std::shared_ptr<Material> checkerCubeMat;
	std::shared_ptr<Material> conveyorMat;
	std::shared_ptr<Material> playerOneMat;
	std::shared_ptr<Material> playerTwoMat;
	std::shared_ptr<Material> playerThreeMat;
	std::shared_ptr<Material> playerFourMat;
	std::shared_ptr<Material> plateMat;
	std::shared_ptr<Material> orangeMat;
	std::shared_ptr<Material> bombMat;

	std::shared_ptr<Shader> TPShader;

	std::shared_ptr<Material> material;

	std::shared_ptr<VertexArray> cube, pyramid, geo;

	std::shared_ptr<CameraController> m_eulerCam; //!< An euler camera that can be moved and rotated
	SceneWideUniforms m_swu2D;
	glm::mat4 view2D;
	glm::mat4 projection2D;

	entt::registry& m_registry;
	std::vector<entt::entity>& m_entities; 

	std::shared_ptr<FrameBuffer> m_sceneFBO;
	std::shared_ptr<FrameBuffer> m_defaultFBO;
	std::shared_ptr<FrameBuffer> m_mergeFBO;
	Quad m_screenQuad;
	SubTexture m_screenTexture;
	SubTexture m_mergetexture;
	bool m_mouseOverScene;
	glm::vec4 col;

	float movetime = 4.75f;

	enum State {InitialMove, Selection, Movement, CheckPoints};
	State currentstate = InitialMove;
	int movetriggers = 0;
	int playerobjects[4] = { 46, 48, 50, 52 };

	void CreateNewPlayer(const char* name, int keypress, int arrayid);
	void GenerateArrayPos(std::array<uint32_t, 8>& objectid);

	void InitialState(float timestep);
	void SelectionState(float timestep);
	void MovementState(float timestep);
	void CheckState();
};
