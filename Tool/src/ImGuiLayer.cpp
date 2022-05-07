/* \file sceneLayer.cpp */
#include "ImGuiLayer.h"
#include "json.hpp"
#include <fstream>
#include "movementScript.h"

	ImGuiLayer::ImGuiLayer(const char* name) : Layer(name), m_registry(Application::getInstance().getRegistry()), m_entities(Application::getInstance().getEntities())
	{
		//ImGuiHelper::init();

		Renderer3D::init();
		Renderer2D::init();
		
#pragma region SHADERS

		TPShader.reset(Shader::create("./assets/shaders/texturedPhong.glsl"));
		Renderer3D::initShader(TPShader);

#pragma endregion

		auto& window = Application::getInstance().getWindow();

		CameraParams p;

		p.width = window->getWidth();
		p.height = window->getHeight();
		m_eulerCam.reset(new CameraController(p));

		m_swu["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_eulerCam->getCamera().view)));
		m_swu["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_eulerCam->getCamera().projection)));
		m_swu["u_lightColour"] = std::pair<ShaderDataType, void*>(ShaderDataType::Float3, static_cast<void*>(glm::value_ptr(lightData[0])));
		m_swu["u_lightPos"] = std::pair<ShaderDataType, void*>(ShaderDataType::Float3, static_cast<void*>(glm::value_ptr(lightData[1])));
		m_swu["u_viewPos"] = std::pair<ShaderDataType, void*>(ShaderDataType::Float3, static_cast<void*>(glm::value_ptr(lightData[2])));

		view2D = glm::mat4(1.f);
		projection2D = glm::ortho(0.f, static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()), 0.f);
		m_swu2D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(view2D)));
		m_swu2D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(projection2D)));

		RenderCommands::setClearColourCommand(0.7f, 0.7f, 0.7f, 1.f)->action();

#pragma region TEXTURES

		std::shared_ptr<Texture> letterTexture;
		letterTexture.reset(Texture::create("assets/textures/letterCube.png"));
		std::shared_ptr<Texture> numberTexture;
		numberTexture.reset(Texture::create("assets/textures/numberCube.png"));
		std::shared_ptr<Texture> checkerTexture;
		checkerTexture.reset(Texture::create("assets/textures/checkerBoard.png"));

		SubTexture letterSubTexture(letterTexture, glm::vec2(0.f), glm::vec2(1.f));
		SubTexture ESubTexture(letterTexture, glm::vec2(0.33f, 0.5f), glm::vec2(0.66, 1.f));
		SubTexture numberSubTexture(letterTexture, glm::vec2(0.f), glm::vec2(1.f));

#pragma endregion

#pragma region RAW_DATA

		std::vector<Renderer3DVertex> cubeVertices =
		{
			Renderer3DVertex({ 0.5f,  0.5f, -0.5f }, { 0.f,  0.f, -1.f }, { 0.f,   0.f }),
			Renderer3DVertex({ 0.5f, -0.5f, -0.5f }, { 0.f,  0.f, -1.f }, { 0.f,   0.5f }),
			Renderer3DVertex({ -0.5f, -0.5f, -0.5f }, { 0.f,  0.f, -1.f }, { 0.33f, 0.5f }),
			Renderer3DVertex({ -0.5f,  0.5f, -0.5f }, { 0.f,  0.f, -1.f }, { 0.33f, 0.f }),
			Renderer3DVertex({ -0.5f, -0.5f,  0.5f }, { 0.f,  0.f,  1.f }, { 0.33f, 0.5f }),
			Renderer3DVertex({ 0.5f, -0.5f,  0.5f }, { 0.f,  0.f,  1.f }, { 0.66f, 0.5f }),
			Renderer3DVertex({ 0.5f,  0.5f,  0.5f }, { 0.f,  0.f,  1.f }, { 0.66f, 0.f }),
			Renderer3DVertex({ -0.5f,  0.5f,  0.5f }, { 0.f,  0.f,  1.f }, { 0.33,  0.f }),
			Renderer3DVertex({ -0.5f, -0.5f, -0.5f }, { 0.f, -1.f,  0.f }, { 1.f,   0.f }),
			Renderer3DVertex({ 0.5f, -0.5f, -0.5f }, { 0.f, -1.f,  0.f }, { 0.66f, 0.f }),
			Renderer3DVertex({ 0.5f, -0.5f,  0.5f }, { 0.f, -1.f,  0.f }, { 0.66f, 0.5f }),
			Renderer3DVertex({ -0.5f, -0.5f,  0.5f }, { 0.f, -1.f,  0.f }, { 1.0f,  0.5f }),
			Renderer3DVertex({ 0.5f,  0.5f,  0.5f }, { 0.f,  1.f,  0.f }, { 0.f,   0.5f }),
			Renderer3DVertex({ 0.5f,  0.5f, -0.5f }, { 0.f,  1.f,  0.f }, { 0.f,   1.0f }),
			Renderer3DVertex({ -0.5f,  0.5f, -0.5f }, { 0.f,  1.f,  0.f }, { 0.33f, 1.0f }),
			Renderer3DVertex({ -0.5f,  0.5f,  0.5f }, { 0.f,  1.f,  0.f }, { 0.3f,  0.5f }),
			Renderer3DVertex({ -0.5f,  0.5f,  0.5f }, { -1.f,  0.f,  0.f }, { 0.66f, 0.5f }),
			Renderer3DVertex({ -0.5f,  0.5f, -0.5f }, { -1.f,  0.f,  0.f }, { 0.33f, 0.5f }),
			Renderer3DVertex({ -0.5f, -0.5f, -0.5f }, { -1.f,  0.f,  0.f }, { 0.33f, 1.0f }),
			Renderer3DVertex({ -0.5f, -0.5f,  0.5f }, { -1.f,  0.f,  0.f }, { 0.66f, 1.0f }),
			Renderer3DVertex({ 0.5f, -0.5f, -0.5f }, { 1.f,  0.f,  0.f }, { 1.0f,  1.0f }),
			Renderer3DVertex({ 0.5f,  0.5f, -0.5f }, { 1.f,  0.f,  0.f }, { 1.0f,  0.5f }),
			Renderer3DVertex({ 0.5f,  0.5f,  0.5f }, { 1.f,  0.f,  0.f }, { 0.66f, 0.5f }),
			Renderer3DVertex({ 0.5f, -0.5f,  0.5f }, { 1.f,  0.f,  0.f }, { 0.66f, 1.0f })
		};

		std::vector<Renderer3DVertex> pyramidVertices =
		{
			Renderer3DVertex({ -0.5f, -0.5f, -0.5f }, { 0.0f, -1.f, 0.0f }, { 0.0f, 0.0f }),
			Renderer3DVertex({ 0.5f, -0.5f, -0.5f }, { 0.0f, -1.f, 0.0f }, { 0.0f, 0.5f }),
			Renderer3DVertex({ 0.5f, -0.5f,  0.5f }, { 0.0f, -1.f, 0.0f }, { 0.33f, 0.5f }),
			Renderer3DVertex({ -0.5f, -0.5f,  0.5f }, { 0.0f, -1.f, 0.0f }, { 0.33f, 0.0f }),
			Renderer3DVertex({ -0.5f, -0.5f, -0.5f }, { -0.8944f, 0.4472f, 0.f }, { 0.33f, 1.0f }),
			Renderer3DVertex({ -0.5f, -0.5f,  0.5f }, { -0.8944f, 0.4472f, 0.f }, { 0.66f, 1.0f }),
			Renderer3DVertex({ 0.0f,  0.5f,  0.0f }, { -0.8944f, 0.4472f, 0.f }, { 0.5f, 0.0f }),
			Renderer3DVertex({ -0.5f, -0.5f,  0.5f }, { 0.0f, 0.4472f, 0.8944f }, { 0.0f, 0.0f }),
			Renderer3DVertex({ 0.5f, -0.5f,  0.5f }, { 0.0f, 0.4472f, 0.8944f }, { 0.0f, 0.0f }),
			Renderer3DVertex({ 0.0f,  0.5f,  0.0f }, { 0.0f, 0.4472f, 0.8944f }, { 0.0f, 0.0f }),
			Renderer3DVertex({ 0.5f, -0.5f,  0.5f }, { 0.8944f, 0.4472f, 0.0f }, { 0.0f, 0.0f }),
			Renderer3DVertex({ 0.5f, -0.5f, -0.5f }, { 0.8944f, 0.4472f, 0.0f }, { 0.0f, 0.0f }),
			Renderer3DVertex({ 0.0f,  0.5f,  0.0f }, { 0.8944f, 0.4472f, 0.0f }, { 0.0f, 0.0f }),
			Renderer3DVertex({ 0.5f, -0.5f, -0.5f }, { 0.f, 0.4472f, -0.8944f }, { 0.0f, 0.0f }),
			Renderer3DVertex({ -0.5f, -0.5f, -0.5f }, { 0.f, 0.4472f, -0.8944f }, { 0.0f, 0.0f }),
			Renderer3DVertex({ 0.0f,  0.5f,  0.0f }, { 0.f, 0.4472f, -0.8944f }, { 0.0f, 0.0f })
		};

		std::vector<uint32_t> pyramidIndices =
		{
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			7, 8, 9,
			10, 11, 12,
			13, 14, 15
		};

		std::vector <uint32_t> cubeIndices =
		{
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			6, 7, 4,
			8, 9, 10,
			10, 11, 8,
			12, 13, 14,
			14, 15, 12,
			16, 17, 18,
			18, 19, 16,
			20, 21, 22,
			22, 23, 20
		};

#pragma endregion

#pragma region GL_BUFFERS

		std::shared_ptr<VertexBuffer> cubeVBO;
		std::shared_ptr<IndexBuffer> cubeIBO;

		std::shared_ptr<VertexBuffer> pyramidVBO;
		std::shared_ptr<IndexBuffer> pyramidIBO;

		cube.reset(VertexArray::create());
		cubeVBO.reset(VertexBuffer::create(cubeVertices.data(), sizeof(Renderer3DVertex)* cubeVertices.size(), Renderer3DVertex::getLayout()));
		cubeIBO.reset(IndexBuffer::create(cubeIndices.data(), 36));

		cube->addVertexBuffer(cubeVBO);
		cube->setIndexBuffer(cubeIBO);

		pyramid.reset(VertexArray::create());
		pyramidVBO.reset(VertexBuffer::create(pyramidVertices.data(), sizeof(Renderer3DVertex)* pyramidVertices.size(), Renderer3DVertex::getLayout()));
		pyramidIBO.reset(IndexBuffer::create(pyramidIndices.data(), 18));

		pyramid->addVertexBuffer(pyramidVBO);
		pyramid->setIndexBuffer(pyramidIBO);

		cubeVertices.clear();
		cubeIndices.clear();
		pyramidVertices.clear();
		pyramidIndices.clear();

#pragma endregion

#pragma region MATERIALS

		pyramidMat.reset(new Material(TPShader, { 0.4f, 0.7f, 0.3f, 1.f }));
		letterCubeMat.reset(new Material(TPShader, letterTexture));
		numberCubeMat.reset(new Material(TPShader, numberTexture));
		checkerCubeMat.reset(new Material(TPShader, checkerTexture));
		conveyorMat.reset(new Material(TPShader, { 0.5f, 0.5f, 0.5f, 1.f }));

#pragma endregion

#pragma region FRAMEBUFFERS

		FrameBufferLayout FBlayout = { { AttachmentType::Colour, true }, { AttachmentType::Depth, false } };
		m_sceneFBO.reset(FrameBuffer::create({ window->getWidth(), window->getHeight() }, FBlayout));
		m_defaultFBO.reset(FrameBuffer::createDefault());

		m_swu2D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(view2D)));
		m_swu2D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(projection2D)));

		//m_screenQuad = Quad::createCentreHalfExtents({ window->getWidth() / 2, window->getHeight() / 2 }, { window->getWidth() / 2, window->getHeight() / 2 });
		//m_screenTexture = SubTexture(m_sceneFBO->getTarget(0), { 0,1 }, { 1,0 });

#pragma endregion

		m_entities.resize(100);

		m_entities[0] = m_registry.create();
		m_registry.emplace<RootComponent>(m_entities[0]);
		m_registry.emplace<LabelComponent>(m_entities[0], "Root");
		m_registry.emplace<TransformComponent>(m_entities[0]);

		m_entities[1] = m_registry.create();
		m_registry.emplace<LabelComponent>(m_entities[1], "Ground");
		m_registry.emplace<TransformComponent>(m_entities[1], glm::vec3(0.f, -0.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(7.f, 0.25f, 7.f));
		m_registry.emplace<RenderComponent>(m_entities[1], cube, checkerCubeMat);

		glm::vec3 cubeDims(1.f, 0.25f, 1.f);

		float t = 0;
		uint32_t cubeCount = 32;
		float deltaT = 1.0f / static_cast<float>(cubeCount);
		for (uint32_t i = 2; i < cubeCount + 2; i++)
		{
			m_entities[i] = m_registry.create();
			m_registry.emplace<LabelComponent>(m_entities[i], (std::string("Platform Cube ") + std::to_string(i)).c_str());
			m_registry.emplace<TransformComponent>(m_entities[i], glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), cubeDims);
			m_registry.emplace<RenderComponent>(m_entities[i], cube, conveyorMat);

			auto& nsc = m_registry.emplace<NativeScriptComponent>(m_entities[i]);
			nsc.create<MovementScript>(m_entities[i], t);
			t += deltaT;
		}
	}

	void ImGuiLayer::onRender()
	{
		m_swu["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_eulerCam->getCamera().view)));
		m_swu["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_eulerCam->getCamera().projection)));	
		
		m_sceneFBO->use();

		RenderCommands::clearDepthAndColourBufferCommand()->action();
		RenderCommands::setDepthTestCommand(true)->action();
		//RenderCommands::setBlendCommand(false)->action();

		Renderer3D::begin(m_swu);

		auto& view = m_registry.view<TransformComponent, RenderComponent>();

		for (auto& entity : view)
		{
			auto& tc = m_registry.get<TransformComponent>(entity);
			auto& rc = m_registry.get<RenderComponent>(entity);

			//Renderer3D::submit(rc.geometry, rc.material, tc.getTransform());

			//if (rc.geometry == cube) rc.material = conveyorMat;
			//rc.material->setTint(rc.material->getTint() + glm::vec4(0.0001f, 0.0f, 0.0f, 0.0f));
		}
		
		std::shared_ptr<Material> colour;
		colour.reset(new Material(TPShader, glm::vec4(1.0f, 1.0f, 0.4f, 1.0f)));
		Renderer3D::submit(cube, colour, glm::mat4(1));

		Renderer3D::end();

		m_defaultFBO->use();

		RenderCommands::clearDepthAndColourBufferCommand()->action();

		ImGuiHelper::begin();

		ImGui::Begin("Renderer Output");
		uint32_t textureID = m_sceneFBO->getTarget(0)->getID();
		ImGui::Image((void*)textureID, ImVec2{ 800, 600 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		if (ImGui::IsWindowHovered()) m_mouseOverScene = true;
		else m_mouseOverScene = false;
		ImGui::End();

		ImGui::ShowDemoWindow();

		ImGui::Begin("Properties");
		ImGui::BeginGroup();
		ImGui::TextWrapped("File:");
		ImGui::SameLine();
		if (ImGui::Button("Load")) loadJSONfile();
		ImGui::SameLine();
		if (ImGui::Button("Save")) saveJSONfile();
		ImGui::EndGroup();
		ImGui::TextWrapped("Object tints:");

		//static ImVec4 checkerCol = { checkerCubeMat->getTint().r, checkerCubeMat->getTint().g, checkerCubeMat->getTint().b, checkerCubeMat->getTint().a };
		ImGui::ColorEdit4(" : Floor",(float*)&colour);
		//checkerCubeMat->setTint(glm::vec4(checkerCol.x, checkerCol.y, checkerCol.z, checkerCol.w));

		ImGui::ColorEdit4(" : Conveyor", &conveyorMat->getTint()[0]);


		ImGui::End();

		ImGuiHelper::end();

		//RenderCommands::setDepthTestCommand(false)->action();
		//RenderCommands::setBlendCommand(true)->action();
		//RenderCommands::setTransparencyBlend()->action();

		//Renderer2D::begin(m_swu2D);

		//Renderer2D::submit(m_screenQuad, m_screenTexture);
		//Renderer2D::submit("2D Renderer", { 300.f, 780.f }, { 1.0,1.0,1.0,1.0 });

		//Renderer2D::end();
	}

	void ImGuiLayer::onUpdate(float timestep)
	{
		if (m_mouseOverScene)
		{
			m_eulerCam->onUpdate(timestep);
			m_swu["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_eulerCam->getCamera().view)));
			m_swu2D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(view2D)));

			auto& view = m_registry.view<NativeScriptComponent>();
			for (auto& entity : view)
			{
				auto& nsc = m_registry.get<NativeScriptComponent>(entity);
				nsc.onUpdate(timestep);
			}
		}		
	}

	void ImGuiLayer::onKeyPressed(KeyPressedEvent& e)
	{
		glm::vec3 forward, right;	
	}

	void ImGuiLayer::onResize(WindowResizeEvent& e)
	{
		m_eulerCam->onResize(e);
	}

	bool ImGuiLayer::loadJSONfile()
	{
		nlohmann::json jsonData;
		std::ifstream handle("./assets/json/settings.json");

		if (handle.is_open())
		{
			handle >> jsonData;

			if (jsonData.count("conveyor") > 0)
			{
				auto& conveyor = jsonData["conveyor"];
				if (conveyor.count("tint") > 0)
				{
					glm::vec4 loadedTint = conveyorMat->getTint();
					auto& tint = conveyor["tint"];
					if (tint.count("r") > 0) loadedTint.r = tint["r"].get<float>();
					if (tint.count("g") > 0) loadedTint.g = tint["g"].get<float>();
					if (tint.count("b") > 0) loadedTint.b = tint["b"].get<float>();
					if (tint.count("a") > 0) loadedTint.a = tint["a"].get<float>();
					conveyorMat->setTint(loadedTint);
				}
			}

			handle.close();
			Log::info("JSON file loaded.");
		}
		else
		{
			Log::info("Could not open JSON file.");
		}
		return true;
	}

	bool ImGuiLayer::saveJSONfile()
	{
		nlohmann::json jsonData;

		jsonData["conveyor"]["tint"]["r"] = conveyorMat->getTint().r;
		jsonData["conveyor"]["tint"]["g"] = conveyorMat->getTint().g;
		jsonData["conveyor"]["tint"]["b"] = conveyorMat->getTint().b;
		jsonData["conveyor"]["tint"]["a"] = conveyorMat->getTint().a;

		std::ofstream handle("./assets.json/settings.json");
		handle << jsonData;
		handle.close();

		Log::info("JSON file saved");
		return true;
	}




