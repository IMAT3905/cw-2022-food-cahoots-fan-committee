/* \file AssimpLayer.cpp */
#include "sceneLayer.h"
#include "movementScript.h"
#include "assimpLoader.h"
#include "player.h"
#include <ctime> 
#include <array>

SceneLayer::SceneLayer(const char* name) : Layer(name), m_registry(Application::getInstance().getRegistry()), m_entities(Application::getInstance().getEntities())
{
	Renderer3D::init();
	Renderer2DBillboard::init(8192);

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
	cubeVBO.reset(VertexBuffer::create(cubeVertices.data(), sizeof(Renderer3DVertex) * cubeVertices.size(), Renderer3DVertex::getLayout()));
	cubeIBO.reset(IndexBuffer::create(cubeIndices.data(), 36));

	cube->addVertexBuffer(cubeVBO);
	cube->setIndexBuffer(cubeIBO);

	pyramid.reset(VertexArray::create());
	pyramidVBO.reset(VertexBuffer::create(pyramidVertices.data(), sizeof(Renderer3DVertex) * pyramidVertices.size(), Renderer3DVertex::getLayout()));
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

	m_swu2D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(view2D)));
	m_swu2D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(projection2D)));

	m_screenQuad = Quad::createCentreHalfExtents({ window->getWidth() / 2, window->getHeight() / 2 }, { window->getWidth() / 2, window->getHeight() / 2 });
	m_screenTexture = SubTexture(textureTarget->getTarget(0), { 0,1 }, { 1,0 });

#pragma endregion

	m_entities.reserve(100);
	
	m_entities.push_back(m_registry.create());
	m_registry.emplace<RootComponent>(m_entities[0]);
	m_registry.emplace<LabelComponent>(m_entities[0], "Root");
	m_registry.emplace<TransformComponent>(m_entities[0]);

	m_entities.push_back(m_registry.create());
	m_registry.emplace<LabelComponent>(m_entities[1], "Ground");
	m_registry.emplace<TransformComponent>(m_entities[1], glm::vec3(0.f, -0.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(7.f, 0.25f, 7.f));
	m_registry.emplace<RenderComponent>(m_entities[1], cube, checkerCubeMat);

	CreateNewPlayer("Player1", NG_KEY_Z, 0);
	CreateNewPlayer("Player2", NG_KEY_X, 1);
	CreateNewPlayer("Player3", NG_KEY_C, 2);
	CreateNewPlayer("Player4", NG_KEY_V, 3);

	uint32_t entcount = 6;

	glm::vec3 cubeDims(1.f, 0.25f, 1.f);

	float t = 0;
	uint32_t cubeCount = 32;
	float deltaT = 1.0f / static_cast<float>(cubeCount);
	for (uint32_t i = entcount; i < cubeCount + entcount; i++)
	{
		m_entities.push_back(m_registry.create());
		m_registry.emplace<LabelComponent>(m_entities.back(), "Platform Cube" + i);
		m_registry.emplace<TransformComponent>(m_entities.back(), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), cubeDims);
		m_registry.emplace<RenderComponent>(m_entities.back(), cube, conveyorMat);

		auto& nsc = m_registry.emplace<NativeScriptComponent>(m_entities.back());
		nsc.create<MovementScript>(m_entities.back(), t, 0.f);
		t += deltaT;
	}

	entcount += cubeCount;

	t = 0;
	uint32_t platecount = 8;
	deltaT = 1.0f / static_cast<float>(platecount);
	float height = 0.2f;

	//Plate
	Loader::ASSIMPLoad("./assets/models/Plate_working/plateTest.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate /* | aiProcess_GenSmoothNormals*/, material, geo);
	material = Loader::s_material;
	material->setShader(TPShader);
	geo = Loader::s_geometry;

	for (uint32_t i = entcount; i < platecount + entcount; i++)
	{
		m_entities.push_back(m_registry.create());
		m_registry.emplace<LabelComponent>(m_entities.back(), "Plate");
		m_registry.emplace<TransformComponent>(m_entities.back(), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1, 1, 1));
		m_registry.emplace<RenderComponent>(m_entities.back(), geo, material);

		auto& nsc = m_registry.emplace<NativeScriptComponent>(m_entities.back());
		nsc.create<MovementScript>(m_entities.back(), t, height);
		t += deltaT;
	}

	entcount += platecount;

	t = 0;
	deltaT = 1.0f / static_cast<float>(platecount);

	//Object
	std::srand(static_cast<unsigned int>(time(nullptr)));
	std::array<uint32_t, 8> objectid = { 0,0,0,0,0,0,0,0 };
	for (int i = 0; i < 3; i++) {
		GenerateArrayPos(objectid);
	}


	Loader::ASSIMPLoad("./assets/models/Orange_OBJ/Orange.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate /* | aiProcess_GenSmoothNormals*/, material, geo);
	material = Loader::s_material;
	material->setShader(TPShader);
	geo = Loader::s_geometry;

	for (uint32_t i = entcount; i < entcount + platecount; i++)
	{
;		m_entities.push_back(m_registry.create());
		switch (objectid[i-entcount]) {
		case 0:
			//Orange
			Loader::ASSIMPLoad("./assets/models/Orange_OBJ/Orange.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate /* | aiProcess_GenSmoothNormals*/, material, geo);
			material = Loader::s_material;
			material->setShader(TPShader);
			geo = Loader::s_geometry;
			m_registry.emplace<LabelComponent>(m_entities.back(), "Orange");
			m_registry.emplace<TransformComponent>(m_entities.back(), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1, 1, 1));
			break;

		case 1:
			//Bomb
			Loader::ASSIMPLoad("./assets/models/Bomb/bomb_OBJ.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate /* | aiProcess_GenSmoothNormals*/, material, geo);
			material = Loader::s_material;
			material->setShader(TPShader);
			geo = Loader::s_geometry;
			m_registry.emplace<LabelComponent>(m_entities.back(), "Bomb");
			m_registry.emplace<TransformComponent>(m_entities.back(), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f));
			break;

		default:
			m_registry.emplace<LabelComponent>(m_entities.back(), "Undefined");
			break;
		}
		m_registry.emplace<RenderComponent>(m_entities.back(), geo, material);
		auto& nsc = m_registry.emplace<NativeScriptComponent>(m_entities.back());
		nsc.create<MovementScript>(m_entities.back(), t, height*2);
		t += deltaT;
	}

	//Left
	Loader::ASSIMPLoad("./assets/models/Player1Model/Player.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate /* | aiProcess_GenSmoothNormals*/, material, geo);
	material = Loader::s_material;
	material->setShader(TPShader);
	geo = Loader::s_geometry;

	m_entities.push_back(m_registry.create());
	m_registry.emplace<LabelComponent>(m_entities.back(), "Player1");
	m_registry.emplace<TransformComponent>(m_entities.back(), glm::vec3(-4.5f, 0.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	m_registry.emplace<RenderComponent>(m_entities.back(), geo, material);

	//Top
	Loader::ASSIMPLoad("./assets/models/Player2Model/Player.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate /* | aiProcess_GenSmoothNormals*/, material, geo);
	material = Loader::s_material;
	material->setShader(TPShader);
	geo = Loader::s_geometry;

	m_entities.push_back(m_registry.create());
	m_registry.emplace<LabelComponent>(m_entities.back(), "Player2");
	m_registry.emplace<TransformComponent>(m_entities.back(), glm::vec3(0.f, 0.5f, -4.5f), glm::vec3(0.f, 4.7f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	m_registry.emplace<RenderComponent>(m_entities.back(), geo, material);

	//Right
	Loader::ASSIMPLoad("./assets/models/Player3Model/Player.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate /* | aiProcess_GenSmoothNormals*/, material, geo);
	material = Loader::s_material;
	material->setShader(TPShader);
	geo = Loader::s_geometry;

	m_entities.push_back(m_registry.create());
	m_registry.emplace<LabelComponent>(m_entities.back(), "Player3");
	m_registry.emplace<TransformComponent>(m_entities.back(), glm::vec3(4.5f, 0.5f, 0.f), glm::vec3(0.f, 9.4f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	m_registry.emplace<RenderComponent>(m_entities.back(), geo, material);

	//Bottom
	Loader::ASSIMPLoad("./assets/models/Player4Model/Player.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate /* | aiProcess_GenSmoothNormals*/, material, geo);
	material = Loader::s_material;
	material->setShader(TPShader);
	geo = Loader::s_geometry;

	m_entities.push_back(m_registry.create());
	m_registry.emplace<LabelComponent>(m_entities.back(), "Player4");
	m_registry.emplace<TransformComponent>(m_entities.back(), glm::vec3(0.0, 0.5f, 4.5f), glm::vec3(0.f, 14.1f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	m_registry.emplace<RenderComponent>(m_entities.back(), geo, material);

	//Quad
	m_entities.push_back(m_registry.create());
	m_registry.emplace<LabelComponent>(m_entities.back(), "BillboardQuad");
	m_registry.emplace<TransformComponent>(m_entities.back(), glm::vec3(2.f, 0.f, -10.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
}

void SceneLayer::onRender()
{
	m_swu["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_eulerCam->getCamera().view)));
	m_swu["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_eulerCam->getCamera().projection)));

	textureTarget->use();

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

	auto t = m_registry.get<TransformComponent>(m_entities.back()).getTransform();
	BillboardQuad quad({ t[3][0], t[3][1] + 1.4, t[3][2] }, { 0.6,0.2 });

	Renderer2DBillboard::begin(m_swu2D);
	Renderer2DBillboard::submit(quad, glm::vec4(1.0f, 0.f, 1.f, 1.f), RendererCommon::defaultSubTexture);
	Renderer2DBillboard::end();

	defaultTarget->use();

	RenderCommands::setDepthTestCommand(false)->action();
	RenderCommands::setBlendCommand(true)->action();
	RenderCommands::setTransparencyBlend()->action();

	Renderer2D::begin(m_swu2D);

	Renderer2D::submit(m_screenQuad, m_screenTexture);

	Renderer2D::end();
}

void SceneLayer::onUpdate(float timestep)
{
	m_eulerCam->onUpdate(timestep);
	m_swu["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_eulerCam->getCamera().view)));
	m_swu2D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(view2D)));

	if (this->isFocused()) {
		switch (currentstate) {
		case InitialMove:
			InitialState(timestep);
			break;

		case Selection:
			SelectionState(timestep);
			break;

		case Movement:
			MovementState(timestep);
			break;

		case CheckPoints:
			CheckState();
			break;
		}
	}
}

void SceneLayer::onKeyPressed(KeyPressedEvent& e)
{
	glm::vec3 forward, right;
	if (e.getKeyCode() == NG_KEY_1) { movetime = 1.66f; Log::info("reset time"); }

	auto& view = m_registry.view<NativeScriptComponent>();
	int i = 0;
	for (auto& entity : view)
	{
		std::string label = m_registry.get<LabelComponent>(entity).label;
		if (label == "Player1" || label == "Player2" || label == "Player3" || label == "Player4") {
			if (currentstate == Selection) {
				auto& nsc = m_registry.get<NativeScriptComponent>(entity);
				nsc.onKeyPress(e);
			}
		}
		else {
			auto& nsc = m_registry.get<NativeScriptComponent>(entity);
			nsc.onKeyPress(e);
		}
	}
}

void SceneLayer::onResize(WindowResizeEvent& e)
{
	m_eulerCam->onResize(e);
}

void SceneLayer::CreateNewPlayer(const char* name, int keypress, int arrayid) {
	m_entities.push_back(m_registry.create());
	m_registry.emplace<LabelComponent>(m_entities.back(), name);

	auto& nsc = m_registry.emplace<NativeScriptComponent>(m_entities.back());
	nsc.create<PlayerScript>(m_entities.back(), keypress, arrayid, *this);
}

void SceneLayer::GenerateArrayPos(std::array<uint32_t, 8>& objectid) {
	uint32_t arrayid = rand() % objectid.size() + 0;
	if (objectid[arrayid] != 1) {
		objectid[arrayid] = 1;
	}
	else {
		GenerateArrayPos(objectid);
	}
}

void SceneLayer::InitialState(float timestep) {
	if (movetime > 0) {
		movetime -= timestep;

		auto& view = m_registry.view<NativeScriptComponent>();
		for (auto& entity : view)
		{
			auto& nsc = m_registry.get<NativeScriptComponent>(entity);
			nsc.onUpdate(timestep);
		}
	}
	else {
		currentstate = Selection;
		selecttime = 5;
		//ID stuff print to console
		//for (int i = 46; i <= 53; i++) {
		//	auto& labelcomp = m_registry.get<LabelComponent>(m_entities[i]);
		//	std::string str = std::to_string(i) + " = " + labelcomp.label;
		//	Log::info(str);
		//}
	}
}

void SceneLayer::SelectionState(float timestep) {
	if (selecttime > 0 ) {
		selecttime -= timestep;
	}
	else {
		for (int i = 0; i < 4; i++) {
			if (numselected[i] == 1) { movetriggers++; }
		}

		if (!Engine::Application::getInstance().GetAudio()->addSound("conveyor", "./assets/audio/conveyor.wav")) {
			Engine::Log::error("Audio file: {0} did not load", "./assets/audio/conveyor.wav");
		}
		Engine::Application::getInstance().GetAudio()->playSound("conveyor");

		movetime = 1.66f * movetriggers;
		currentstate = Movement;
	}
}

void SceneLayer::MovementState(float timestep) {
	if (movetime > 0) {
		movetime -= timestep;

		auto& view = m_registry.view<NativeScriptComponent>();
		for (auto& entity : view)
		{
			auto& nsc = m_registry.get<NativeScriptComponent>(entity);
			nsc.onUpdate(timestep);
		}
	}
	else {
		Engine::Application::getInstance().GetAudio()->StopAllSounds();
		currentstate = CheckPoints;
	}
}

void SceneLayer::CheckState() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < movetriggers; j++) {
			playerobjects[i] -= 1;
			if (playerobjects[i] < 46) { playerobjects[i] = 53; }
		}
	}

	for (int i = 0; i < 4; i++) {
		auto& labelcomp = m_registry.get<LabelComponent>(m_entities[playerobjects[i]]);
		//ID stuff print to console
		//std::string str = std::to_string(playerobjects[i]) + " = " + labelcomp.label;
		//Log::error(str);

		if (labelcomp.label == "Orange") {
			scores[i]++;
		}
		else if (labelcomp.label == "Bomb") {
			scores[i]--;
			if (scores[i] < 0) { scores[i] = 0; }
		}
	}

	currentstate = Selection;
	numselected[0] = 0;
	numselected[1] = 0;
	numselected[2] = 0;
	numselected[3] = 0;
	movetriggers = 0;
	selecttime = 5;

	//Audio
	if (!Engine::Application::getInstance().GetAudio()->addSound("ping", "./assets/audio/ping.wav")) {
		Engine::Log::error("Audio file: {0} did not load", "./assets/audio/ping.wav");
	}
	Engine::Application::getInstance().GetAudio()->playSound("ping");
}