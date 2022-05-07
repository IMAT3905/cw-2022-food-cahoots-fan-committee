/* \file UILayer.cpp */
#include "UILayer.h"

UILayer::UILayer(const char* name) : Layer(name)
{
	Renderer2D::init();

	auto& window = Application::getInstance().getWindow();

	projection2D = glm::ortho(0.f, static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()), 0.f);

	m_swu["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(view2D)));
	m_swu["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(projection2D)));

	m_quads.push_back(Quad::createCentreHalfExtents({ 512.f, 750.f }, { 512.f, 50.f }));

	b2Vec2 gravity(0.0f, 9.81f);
	world = new b2World(gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(500.0f, 600.0f);

	groundBody = world->CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(500.0f, 400.0f);
	body = world->CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(5.0f, 5.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f;

	body->CreateFixture(&fixtureDef);
}

void UILayer::onRender()
{
	RenderCommands::setDepthTestCommand(false)->action();

	RenderCommands::setBlendCommand(true)->action();
	RenderCommands::setTransparencyBlend()->action();

	Renderer2D::begin(m_swu);

	Renderer2D::submit(m_quads[0], { 0.2f, 0.6f, 0.4f, 0.5f });
	Renderer2D::submit("Test Footer", { 300.f, 780.f }, { 0.4f, 0.0f, 0.0f, 0.8f });

	Quad groundQuad = Quad::createCentreHalfExtents({ groundBody->GetPosition().x, groundBody->GetPosition().y }, { 50.0f, 10.0f });
	Quad bodyQuad = Quad::createCentreHalfExtents({ body->GetPosition().x, body->GetPosition().y }, { 5.0f, 5.0f });

	Renderer2D::submit(groundQuad, { 0.8f, 0.2f, 0.2f, 1.0f });
	Renderer2D::submit(bodyQuad, { 0.2f, 0.2f, 0.8f, 1.0f });

	Renderer2D::end();
}

void UILayer::onUpdate(float timestep)
{
	world->Step(timestep, 5, 7);
}
