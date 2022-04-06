/* \file sceneLayer.cpp */
#include "sceneLayer.h"
#include "movementScript.h"

	SceneLayer::SceneLayer(const char* name) : Layer(name), m_registry(Application::getInstance().getRegistry()), m_entities(Application::getInstance().getEntities())
	{
		Renderer3D::init();
		Renderer2D::init();
		
#pragma region SHADERS

		TPShader.reset(Shader::create("./assets/shaders/texturedPhong.glsl"));

#pragma endregion

		auto& window = Application::getInstance().getWindow();

		CameraParams p;

		p.width = window->getWidth();
		p.height = window->getHeight();
		m_eulerCam.reset(new CameraController(p));

		UniformBufferLayout camLayout = { {"u_projection", ShaderDataType::Mat4}, {"u_view", ShaderDataType::Mat4} };
		
		cameraUBO.reset(UniformBuffer::create(camLayout));

		cameraUBO->attachShaderBlock(TPShader, "b_camera");

		cameraUBO->uploadData("u_projection", glm::value_ptr(m_eulerCam->getCamera().projection));
		cameraUBO->uploadData("u_view", glm::value_ptr(m_eulerCam->getCamera().view));

		glm::vec3 lightColour(1.f, 1.f, 1.f);
		glm::vec3 lightPos(1.0f, 4.0f, 6.0f);
		glm::vec3 viewPos(0.0f, 0.0f, 0.0f);

		UniformBufferLayout lightLayout = { {"u_lightPos", ShaderDataType::Float4}, {"u_viewPos", ShaderDataType::Float4}, {"u_lightColour", ShaderDataType::Float4} };

		lightsUBO.reset(UniformBuffer::create(lightLayout));

		lightsUBO->attachShaderBlock(TPShader, "b_lights");
		lightsUBO->uploadData("u_lightPos", glm::value_ptr(lightPos));
		lightsUBO->uploadData("u_viewPos", glm::value_ptr(viewPos));
		lightsUBO->uploadData("u_lightColour", glm::value_ptr(lightColour));

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

		textureTarget.reset(FrameBuffer::create({ window->getWidth(), window->getHeight() }, FBlayout));
		defaultTarget.reset(FrameBuffer::createDefault());

		glm::mat4 view2D = glm::mat4(1.f);
		glm::mat4 projection2D = glm::ortho(0.f, static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()), 0.f);
		m_swu2D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(view2D)));
		m_swu2D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(projection2D)));

		m_screenQuad = Quad::createCentreHalfExtents({ 512.f, 400.f }, { 512.f, 400.f });
		m_screenTexture = SubTexture(textureTarget->getTarget(0), { 0,1 }, { 1,0 });

		//std::vector<Renderer3DVertex>

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

	void SceneLayer::onRender()
	{
		m_swu["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_eulerCam->getCamera().view)));
		m_swu["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_eulerCam->getCamera().projection)));	

		textureTarget->use();

		RenderCommands::clearDepthAndColourBufferCommand()->action();
		RenderCommands::setDepthTestCommand(true)->action();
		RenderCommands::setBlendCommand(false);

		Renderer3D::begin(m_swu);

		auto& view = m_registry.view<TransformComponent, RenderComponent>();

		for (auto& entity : view)
		{
			auto& tc = m_registry.get<TransformComponent>(entity);
			auto& rc = m_registry.get<RenderComponent>(entity);

			Renderer3D::submit(rc.geometry, rc.material, tc.getTransform());
		}
		
		Renderer3D::end();

		defaultTarget->use();

		RenderCommands::setDepthTestCommand(false);
		RenderCommands::setBlendCommand(true);

		Renderer2D::begin(m_swu2D);

		Renderer2D::submit(m_screenQuad, m_screenTexture);
		Renderer2D::submit("2D Renderer", { 300.f, 780.f }, { 1.0,1.0,1.0,1.0 });

		Renderer2D::end();
	}

	void SceneLayer::onUpdate(float timestep)
	{
		m_eulerCam->onUpdate(timestep);
		cameraUBO->uploadData("u_view", glm::value_ptr(m_eulerCam->getCamera().view));

		auto& view = m_registry.view<NativeScriptComponent>();
		for (auto& entity : view)
		{
			auto& nsc = m_registry.get<NativeScriptComponent>(entity);
			nsc.onUpdate(timestep);
		}
	}

	void SceneLayer::onKeyPressed(KeyPressedEvent& e)
	{
		glm::vec3 forward, right;	
	}

	void SceneLayer::onResize(WindowResizeEvent& e)
	{
		m_eulerCam->onResize(e);
	}




