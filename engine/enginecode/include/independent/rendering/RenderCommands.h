/** \file renderCommands.h/ */
#pragma once

#include <functional>

namespace Engine
{
	/** \class RenderCommands
	** \brief A class to hold all the render commands
	*/
	class RenderCommands
	{
	public:
		virtual void action() = 0; //!< A function to use the command

		static RenderCommands* clearDepthAndColourBufferCommand(); //!< A command to clear the depth and colour buffers
		static RenderCommands* clearColourBufferCommand(); //!< A command to clear the colour buffer
		static RenderCommands* clearDepthBufferCommand(); //!< A command to clear the depth buffer
		static RenderCommands* setClearColourCommand(float r, float g, float b, float a); //!< A command to set a clear colour \param r is the red channel \param g is the green channel \param b is the blue channel \param a is the alpha channel
		static RenderCommands* setDepthTestCommand(bool enabled); //!< A command to enable and disable depth testing \param enabled will determine whether depth testing is enabled or disabled
		static RenderCommands* setBlendCommand(bool enabled); //!< A command to enable and disable blending \param enabled will determine whether blending is enabled or disabled
		static RenderCommands* setTransparencyBlend(); //!< A command to enable a transparency blend
		static RenderCommands* setFaceCullCommand(bool enabled); //!< A command to enable and disable backface culling \param enabled will determine whether backface culling is enabled or disabled
		static RenderCommands* clearScreenCommand(float r, float g, float b, float a); //!< A command to clear the depth and colour buffers and then set a clear colour \param r is the red channel \param g is the green channel \param b is the blue channel \param a is the alpha channel
	};
}