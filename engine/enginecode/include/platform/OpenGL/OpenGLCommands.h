/* \file OpenGLCommands.h */
#pragma once

#include "engine_pch.h"
#include <glad/glad.h>
#include "rendering/RenderCommands.h"

namespace Engine
{
	/** \class OpenGLSetClearColourCommand
	** \brief A class for using the set clear colour command
	*/
	class OpenGLSetClearColourCommand : public RenderCommands
	{
	public:
		OpenGLSetClearColourCommand(float r, float g, float b, float a) : m_r(r), m_g(g), m_b(b), m_a(a) {}; //!< using the opengl command set clear colour \param r is the red channel \param g is the green channel \param b is the blue channel \param a is the alpha channel
		void action() override; //!< overriding action with the specific command
	private:
		float m_r, m_g, m_b, m_a; //!< Each of the channels
	};

	void OpenGLSetClearColourCommand::action()
	{
		glClearColor(m_r, m_g, m_b, m_a);
	}

	/** \class OpenGLClearDepthBufferCommand
	** \brief A class for using the clear depth buffer command
	*/
	class OpenGLClearDepthBufferCommand : public RenderCommands
	{
	public:
		OpenGLClearDepthBufferCommand() {}; //!< using the opengl command clear depth buffer
		void action() override; //!< overriding action with the specific command
	};

	void OpenGLClearDepthBufferCommand::action()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	/** \class OpenGLClearColourBufferCommand
	** \brief A class for using the clear colour buffer command
	*/
	class OpenGLClearColourBufferCommand : public RenderCommands
	{
	public:
		OpenGLClearColourBufferCommand() {}; //!< using the opengl command clear colour buffer
		void action() override; //!< overriding action with the specific command
	};

	void OpenGLClearColourBufferCommand::action()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	/** \class OpenGLClearColourAndDepthBufferCommand
	** \brief A class for using the clear colour and depth buffers
	*/
	class OpenGLClearColourAndDepthBufferCommand : public RenderCommands
	{
	public:
		OpenGLClearColourAndDepthBufferCommand() {}; //!< using the opengl command clear colour and depth buffers
		void action() override; //!< overriding action with the specific command
	};

	void OpenGLClearColourAndDepthBufferCommand::action()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	/** \class OpenGLSetDepthTestCommand
	** \brief A class for using the set depth test command
	*/
	class OpenGLSetDepthTestCommand : public RenderCommands
	{
	public:
		OpenGLSetDepthTestCommand(bool enabled) : m_enabled(enabled) {}; //!< using the opengl command set depth test \param enabled is whether the depth test is enabled or disabled
		void action() override; //!< overriding action with the specific command
	private:
		bool m_enabled; //!< used to determine whether the depth test is enabled or disabled
	};

	void OpenGLSetDepthTestCommand::action()
	{
		if (m_enabled) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
	}

	/** \class OpenGLSetBlendCommand
	** \brief A class for using the set blend command
	*/
	class OpenGLSetBlendCommand : public RenderCommands
	{
	public:
		OpenGLSetBlendCommand(bool enabled) : m_enabled(enabled) {}; //!< using the opengl command to set blend command \param enabled is whether the blending is enabled or disabled
		void action() override; //!< overriding action with the specific command
	private:
		bool m_enabled; //!< used to determine whether the set blend command is enabled or disabled
	};

	void OpenGLSetBlendCommand::action()
	{
		if (m_enabled) glEnable(GL_BLEND);
		else glDisable(GL_BLEND);
	}

	/** \class OpenGLSetTransparencyBlendCommand
	** \brief A class for applying transparency when blending
	*/
	class OpenGLSetTransparencyBlendCommand : public RenderCommands
	{
	public:
		OpenGLSetTransparencyBlendCommand() {}; //!< using the opengl command to set a transparency blend function
		void action() override; //!< overriding action with the specific command
	};

	void OpenGLSetTransparencyBlendCommand::action()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	/** \class OpenGLSetFaceCullCommand
	** \brief A class for using the set facecull command
	*/
	class OpenGLSetFaceCullCommand : public RenderCommands
	{
	public:
		OpenGLSetFaceCullCommand(bool enabled) : m_enabled(enabled) {}; //!< using the opengl command to set facecull command \param enabled is whether the faces are culled or not
		void action() override; //!< overriding action with the specific command
	private:
		bool m_enabled; //!< used to determine whether the faces are culled or not
	};

	void OpenGLSetFaceCullCommand::action()
	{
		if (m_enabled) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
	}

	/** \class OpenGLClearScreenCommand
	** \brief A class for clearing the colour and depth buffers and setting a clear colour
	*/
	class OpenGLClearScreenCommand : public RenderCommands
	{
	public:
		OpenGLClearScreenCommand(float r, float g, float b, float a) : m_r(r), m_g(g), m_b(b), m_a(a) {}; //!< using the opengl commands to clear the depth and colour buffers and set a clear colour \param r is the red channel \param g is the green channel \param b is the blue channel \param a is the alpha channel
		void action() override; //!< overriding action with the specific command
	private:
		float m_r, m_g, m_b, m_a; //!< Each of the channels
	};

	void OpenGLClearScreenCommand::action()
	{
		glClearColor(m_r, m_g, m_b, m_a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}