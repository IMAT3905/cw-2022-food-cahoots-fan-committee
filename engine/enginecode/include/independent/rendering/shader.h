/* \file shader.h */
#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace Engine
{
	/** \class Shader
	** \brief API agnostic shader class
	*/
	class Shader
	{
	public:
		virtual ~Shader() = default; //!< Destructor
		virtual uint32_t getID() const = 0; //!< function to return the ID
 
		virtual void uploadInt(const char* name, int value) = 0; //!< function to upload an int
		virtual void uploadIntArray(const char* name, int32_t* values, uint32_t count) = 0; //!< function to upload an int array
		virtual void uploadFloat(const char* name, float value) = 0; //!< function to upload an float
		virtual void uploadFloat2(const char* name, const glm::vec2& value) = 0; //!< function to upload a vec2
		virtual void uploadFloat3(const char* name, const glm::vec3& value) = 0; //!< function to upload a vec3
		virtual void uploadFloat4(const char* name, const glm::vec4& value) = 0; //!< function to upload a vec4
		virtual void uploadMat4(const char* name, const glm::mat4& value) = 0; //!< function to upload a mat4

		static Shader* createMult(const char* vertexFilepath, const char* fragmentFilepath); //!< function to link a vertex and a fragment shader
		static Shader* create(const char* filepath); //!< function to link a single shader
	};
}