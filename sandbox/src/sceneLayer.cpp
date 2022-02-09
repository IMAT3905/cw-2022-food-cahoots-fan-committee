/* \file sceneLayer.cpp */
#include "sceneLayer.h"
#include "movementScript.h"

	/** \class TPVertexNormalised
		** \brief Class which uses a textured phong and has normalised data
		*/
	class TPVertexNormalised
	{
	public:
		glm::vec3 m_pos; //!< The position data
		std::array<int16_t, 3> m_normal; //!< The normal data
		std::array<int16_t, 2> m_uv; //!< The UV data

		TPVertexNormalised() : m_pos(glm::vec3(0.f)), m_normal({ 0, 0, 0 }), m_uv({ 0, 0 }) {} //!< Default constructor
		TPVertexNormalised(const glm::vec3& pos, const std::array<int16_t, 3>& normal, const std::array<int16_t, 2>& uv) : m_pos(pos), m_normal(normal), m_uv(uv) {} //!< Constructor \param pos is the position of the object \param normal is the normal data for the object \param uv is the uv data for the object
		static VertexBufferLayout getLayout() { return s_layout; } //!< Return the layout
	private:
		static VertexBufferLayout s_layout; //!< The layout of the data
	};

	VertexBufferLayout TPVertexNormalised::s_layout = { {ShaderDataType::Float3, {ShaderDataType::Short3, true}, {ShaderDataType::Short2, true} }, 24 }; //!< Layout for data in TPVertexNormalised

	std::array<int16_t, 3> normalise(const glm::vec3& normal)
	{
		std::array<int16_t, 3> result;

		if (normal.x == 1.0) result.at(0) = INT16_MAX;
		else if (normal.x == -1.0) result.at(0) = INT16_MIN;
		else result.at(0) = static_cast<int16_t>(normal.x * static_cast<float>(INT16_MAX));

		if (normal.y == 1.0) result.at(1) = INT16_MAX;
		else if (normal.y == -1.0) result.at(1) = INT16_MIN;
		else result.at(1) = static_cast<int16_t>(normal.y * static_cast<float>(INT16_MAX));

		if (normal.z == 1.0) result.at(2) = INT16_MAX;
		else if (normal.z == -1.0) result.at(2) = INT16_MIN;
		else result.at(2) = static_cast<int16_t>(normal.z * static_cast<float>(INT16_MAX));

		return result;
	};

	std::array<int16_t, 2> normalise(const glm::vec2& uv)
	{
		std::array<int16_t, 2> result;

		if (uv.x == 1.0) result.at(0) = INT16_MAX;
		else if (uv.x == -1.0) result.at(0) = INT16_MIN;
		else result.at(0) = static_cast<int16_t>(uv.x * static_cast<float>(INT16_MAX));

		if (uv.y == 1.0) result.at(1) = INT16_MAX;
		else if (uv.y == -1.0) result.at(1) = INT16_MIN;
		else result.at(1) = static_cast<int16_t>(uv.y * static_cast<float>(INT16_MAX));

		return result;
	};

	SceneLayer::SceneLayer(const char* name) : Layer(name), m_registry(Application::getInstance().getRegistry()), m_entities(Application::getInstance().getEntities())
	{
		Renderer3D::init();
		
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

		std::vector<TPVertexNormalised> cubeVertices(24);

		cubeVertices.at(0) = TPVertexNormalised({ 0.5f,  0.5f, -0.5f }, normalise({ 0.f,  0.f, -1.f }), normalise({ 0.f,   0.f }));
		cubeVertices.at(1) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f }, normalise({ 0.f,  0.f, -1.f }), normalise({ 0.f,   0.5f }));
		cubeVertices.at(2) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, normalise({ 0.f,  0.f, -1.f }), normalise({ 0.33f, 0.5f }));
		cubeVertices.at(3) = TPVertexNormalised({ -0.5f,  0.5f, -0.5f }, normalise({ 0.f,  0.f, -1.f }), normalise({ 0.33f, 0.f }));
		cubeVertices.at(4) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, normalise({ 0.f,  0.f,  1.f }), normalise({ 0.33f, 0.5f }));
		cubeVertices.at(5) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f }, normalise({ 0.f,  0.f,  1.f }), normalise({ 0.66f, 0.5f }));
		cubeVertices.at(6) = TPVertexNormalised({ 0.5f,  0.5f,  0.5f }, normalise({ 0.f,  0.f,  1.f }), normalise({ 0.66f, 0.f }));
		cubeVertices.at(7) = TPVertexNormalised({ -0.5f,  0.5f,  0.5f }, normalise({ 0.f,  0.f,  1.f }), normalise({ 0.33,  0.f }));
		cubeVertices.at(8) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, normalise({ 0.f, -1.f,  0.f }), normalise({ 1.f,   0.f }));
		cubeVertices.at(9) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f }, normalise({ 0.f, -1.f,  0.f }), normalise({ 0.66f, 0.f }));
		cubeVertices.at(10) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f }, normalise({ 0.f, -1.f,  0.f }), normalise({ 0.66f, 0.5f }));
		cubeVertices.at(11) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, normalise({ 0.f, -1.f,  0.f }), normalise({ 1.0f,  0.5f }));
		cubeVertices.at(12) = TPVertexNormalised({ 0.5f,  0.5f,  0.5f }, normalise({ 0.f,  1.f,  0.f }), normalise({ 0.f,   0.5f }));
		cubeVertices.at(13) = TPVertexNormalised({ 0.5f,  0.5f, -0.5f }, normalise({ 0.f,  1.f,  0.f }), normalise({ 0.f,   1.0f }));
		cubeVertices.at(14) = TPVertexNormalised({ -0.5f,  0.5f, -0.5f }, normalise({ 0.f,  1.f,  0.f }), normalise({ 0.33f, 1.0f }));
		cubeVertices.at(15) = TPVertexNormalised({ -0.5f,  0.5f,  0.5f }, normalise({ 0.f,  1.f,  0.f }), normalise({ 0.3f,  0.5f }));
		cubeVertices.at(16) = TPVertexNormalised({ -0.5f,  0.5f,  0.5f }, normalise({ -1.f,  0.f,  0.f }), normalise({ 0.66f, 0.5f }));
		cubeVertices.at(17) = TPVertexNormalised({ -0.5f,  0.5f, -0.5f }, normalise({ -1.f,  0.f,  0.f }), normalise({ 0.33f, 0.5f }));
		cubeVertices.at(18) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, normalise({ -1.f,  0.f,  0.f }), normalise({ 0.33f, 1.0f }));
		cubeVertices.at(19) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, normalise({ -1.f,  0.f,  0.f }), normalise({ 0.66f, 1.0f }));
		cubeVertices.at(20) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f }, normalise({ 1.f,  0.f,  0.f }), normalise({ 1.0f,  1.0f }));
		cubeVertices.at(21) = TPVertexNormalised({ 0.5f,  0.5f, -0.5f }, normalise({ 1.f,  0.f,  0.f }), normalise({ 1.0f,  0.5f }));
		cubeVertices.at(22) = TPVertexNormalised({ 0.5f,  0.5f,  0.5f }, normalise({ 1.f,  0.f,  0.f }), normalise({ 0.66f, 0.5f }));
		cubeVertices.at(23) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f }, normalise({ 1.f,  0.f,  0.f }), normalise({ 0.66f, 1.0f }));

		std::vector<TPVertexNormalised> pyramidVertices(16);

		pyramidVertices.at(0) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, normalise({ 0.0f, -1.f, 0.0f }), normalise({ 0.0f, 0.0f }));
		pyramidVertices.at(1) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f }, normalise({ 0.0f, -1.f, 0.0f }), normalise({ 0.0f, 0.5f }));
		pyramidVertices.at(2) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f }, normalise({ 0.0f, -1.f, 0.0f }), normalise({ 0.33f, 0.5f }));
		pyramidVertices.at(3) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, normalise({ 0.0f, -1.f, 0.0f }), normalise({ 0.33f, 0.0f }));
		pyramidVertices.at(4) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, normalise({ -0.8944f, 0.4472f, 0.f }), normalise({ 0.33f, 1.0f }));
		pyramidVertices.at(5) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, normalise({ -0.8944f, 0.4472f, 0.f }), normalise({ 0.66f, 1.0f }));
		pyramidVertices.at(6) = TPVertexNormalised({ 0.0f,  0.5f,  0.0f }, normalise({ -0.8944f, 0.4472f, 0.f }), normalise({ 0.5f, 0.0f }));
		pyramidVertices.at(7) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, normalise({ 0.0f, 0.4472f, 0.8944f }), normalise({ 0.0f, 0.0f }));
		pyramidVertices.at(8) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f }, normalise({ 0.0f, 0.4472f, 0.8944f }), normalise({ 0.0f, 0.0f }));
		pyramidVertices.at(9) = TPVertexNormalised({ 0.0f,  0.5f,  0.0f }, normalise({ 0.0f, 0.4472f, 0.8944f }), normalise({ 0.0f, 0.0f }));
		pyramidVertices.at(10) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f }, normalise({ 0.8944f, 0.4472f, 0.0f }), normalise({ 0.0f, 0.0f }));
		pyramidVertices.at(11) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f }, normalise({ 0.8944f, 0.4472f, 0.0f }), normalise({ 0.0f, 0.0f }));
		pyramidVertices.at(12) = TPVertexNormalised({ 0.0f,  0.5f,  0.0f }, normalise({ 0.8944f, 0.4472f, 0.0f }), normalise({ 0.0f, 0.0f }));
		pyramidVertices.at(13) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f }, normalise({ 0.f, 0.4472f, -0.8944f }), normalise({ 0.0f, 0.0f }));
		pyramidVertices.at(14) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, normalise({ 0.f, 0.4472f, -0.8944f }), normalise({ 0.0f, 0.0f }));
		pyramidVertices.at(15) = TPVertexNormalised({ 0.0f,  0.5f,  0.0f }, normalise({ 0.f, 0.4472f, -0.8944f }), normalise({ 0.0f, 0.0f }));

		uint32_t pyramidIndices[3 * 6] =
		{
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			7, 8, 9,
			10, 11, 12,
			13, 14, 15
		};

		uint32_t cubeIndices[3 * 12] = {
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

		cubeVAO.reset(VertexArray::create());
		cubeVBO.reset(VertexBuffer::create(cubeVertices.data(), sizeof(TPVertexNormalised) * cubeVertices.size(), TPVertexNormalised::getLayout()));
		cubeIBO.reset(IndexBuffer::create(cubeIndices, 36));

		cubeVAO->addVertexBuffer(cubeVBO);
		cubeVAO->setIndexBuffer(cubeIBO);

		pyramidVAO.reset(VertexArray::create());
		pyramidVBO.reset(VertexBuffer::create(pyramidVertices.data(), sizeof(TPVertexNormalised) * pyramidVertices.size(), TPVertexNormalised::getLayout()));
		pyramidIBO.reset(IndexBuffer::create(pyramidIndices, 18));

		pyramidVAO->addVertexBuffer(pyramidVBO);
		pyramidVAO->setIndexBuffer(pyramidIBO);

#pragma endregion

#pragma region MATERIALS

		pyramidMat.reset(new Material(TPShader, { 0.4f, 0.7f, 0.3f, 1.f }));
		letterCubeMat.reset(new Material(TPShader, letterTexture));
		numberCubeMat.reset(new Material(TPShader, numberTexture));
		checkerCubeMat.reset(new Material(TPShader, checkerTexture));

#pragma endregion

		m_entities.resize(12);

		m_entities[0] = m_registry.create();
		m_registry.emplace<RootComponent>(m_entities[0]);
		m_registry.emplace<LabelComponent>(m_entities[0], "Root");
		m_registry.emplace<TransformComponent>(m_entities[0]);

		m_entities[1] = m_registry.create();
		m_registry.emplace<LabelComponent>(m_entities[1], "Ground");
		m_registry.emplace<TransformComponent>(m_entities[1], glm::vec3(0.f, 0.25f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(6.f, 0.25f, 6.f));
		m_registry.emplace<RenderComponent>(m_entities[1], cubeVAO, checkerCubeMat);

		m_entities[2] = m_registry.create();
		m_registry.emplace<LabelComponent>(m_entities[2], "Letter Cube 1");
		m_registry.emplace<TransformComponent>(m_entities[2], glm::vec3(0.f, 0.f, -6.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
		m_registry.emplace<RenderComponent>(m_entities[2], cubeVAO, letterCubeMat);

		m_entities[3] = m_registry.create();
		m_registry.emplace<LabelComponent>(m_entities[3], "Letter Cube 2");		
		m_registry.emplace<TransformComponent>(m_entities[3], glm::vec3(2.f, 0.f, -6.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));	
		m_registry.emplace<RenderComponent>(m_entities[3], cubeVAO, numberCubeMat);
		
		//auto& nsc = m_registry.emplace<NativeScriptComponent>(m_entities[3]);
		//nsc.create<MovementScript>(m_entities[3]);
	}

	void SceneLayer::onRender()
	{
		m_swu["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_eulerCam->getCamera().view)));
		m_swu["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_eulerCam->getCamera().projection)));	

		RenderCommands::clearDepthAndColourBufferCommand()->action();

		RenderCommands::setDepthTestCommand(true)->action();

		Renderer3D::begin(m_swu);

		auto& view = m_registry.view<TransformComponent, RenderComponent>();

		for (auto& entity : view)
		{
			auto& tc = m_registry.get<TransformComponent>(entity);
			auto& rc = m_registry.get<RenderComponent>(entity);

			Renderer3D::submit(rc.geometry, rc.material, tc.getTransform());
		}

		Renderer3D::end();
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




