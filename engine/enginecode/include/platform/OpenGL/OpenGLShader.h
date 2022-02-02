/* \file OpenGLShader.h */
#pragma once

#include <cstdint>

#include "rendering/shader.h"

namespace Engine
{
	/** \class OpenGLShader
	** \brief A class for loading the shaders and uploading data as uniforms
	*/
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const char* vertexFilepath, const char* fragmentFilepath); //!< Constructor for 2 filepaths \param vertexFilepath is the vertex shader's location \param fragmentFilepath is the fragment shader's location
		OpenGLShader(const char* filepath); //!< Constructor for 1 filepath \param filepath is for a single shader's location
		virtual ~OpenGLShader(); //!< Destructor
		virtual uint32_t getID() const override { return m_OpenGL_ID; } //!< function to return the ID

		virtual void uploadInt(const char* name, int value) override; //!< function to upload an int \param name is the name of the uniform data \param value is the piece of data to be uploaded
		virtual void uploadIntArray(const char* name, int32_t* values, uint32_t count) override; //!< function to upload an int array \param name is the name of the uniform data \param values is the data that will be uploaded \param count is the number of pieces of data
		virtual void uploadFloat(const char* name, float value) override; //!< function to upload an int \param name is the name of the uniform data \param value is the data that will be uploaded
		virtual void uploadFloat2(const char* name, const glm::vec2& value) override; //!< function to upload a vec2 \param name is the name of the uniform data \param value is the data that will be uploaded
		virtual void uploadFloat3(const char* name, const glm::vec3& value) override; //!< function to upload a vec3 \param name is the name of the uniform data \param value is the data that will be uploaded
		virtual void uploadFloat4(const char* name, const glm::vec4& value) override; //!< function to upload a vec4 \param name is the name of the uniform data \param value is the data that will be uploaded
		virtual void uploadMat4(const char* name, const glm::mat4& value) override; //!< function to upload a mat4 \param name is the name of the uniform data \param value is the data that will be uploaded
	private:
		uint32_t m_OpenGL_ID; //!< Render ID
		void compileAndLink(const char* vertexShaderSrc, const char* fragmentShaderSrc); //!< function to compile and link shaders \param vertexShaderSrc is the location of the vertex shader \param fragmentShaderSrc is the location of the fragment shader
	};
}