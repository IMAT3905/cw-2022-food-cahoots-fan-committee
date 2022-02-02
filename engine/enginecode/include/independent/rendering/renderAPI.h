/* \file renderAPI.h*/
#pragma once

namespace Engine
{
	/** \class RenderAPI
	** \brief A class to define each render API
	*/
	class RenderAPI
	{
	public:
		enum class API { None = 0, OpenGL = 1, Direct3D = 2, Vulkan = 3 }; //!< enum to hold different APIs
		inline static API getAPI() { return s_API; } //!< function to return the current API
	private:
		static API s_API; //!< The current API
	};
}
