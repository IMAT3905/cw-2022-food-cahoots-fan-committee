/* \file graphicsContext.h */
#pragma once

namespace Engine
{
	/** \class GraphicsContext
	** \brief Used to initialise the graphics context of the API
	*/
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default; //!< Destuctor
		virtual void init() = 0; //!< Init the graphics context for the given windowing API
		virtual void swapBuffers() = 0; //!< Swap the front and back buffer (double buffering)
		virtual void updateViewport(int32_t width, int32_t height) = 0; //!< Update the viewport
	};
}