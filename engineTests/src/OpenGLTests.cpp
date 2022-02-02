/** \file OpenGLTests.cpp */

#include "OpenGLTests.h"

TEST(OpenGL, VertexArrayConstructor)
{
	Engine::OpenGLVertexArray vao;
	uint32_t id = vao.getID();
	uint32_t count = vao.getDrawCount();
	uint32_t size = vao.getVertexBuffers().size();	

	EXPECT_EQ(id, 1);
	EXPECT_EQ(count, 0);
	EXPECT_EQ(size, 0);
}

TEST(OpenGL, TextureConstructor)
{
	Engine::OpenGLTexture texture(50, 100, 4, nullptr);

	uint32_t id = texture.getID();
	uint32_t width = texture.getWidth();
	uint32_t widthf = texture.getWidthf();
	uint32_t height = texture.getHeight();
	uint32_t heightf = texture.getHeightf();
	uint32_t channels = texture.getChannels();

	EXPECT_EQ(id, 1);
	EXPECT_EQ(width, 50);
	EXPECT_EQ(widthf, 50.f);
	EXPECT_EQ(height, 100);
	EXPECT_EQ(heightf, 100.f);
	EXPECT_EQ(channels, 4);
}