#region Vertex

#version 440 core
			
layout(location = 0) in vec4 a_vertexPosition;
layout(location = 1) in vec2 a_texCoord;

out vec2 texCoord;

layout (std140) uniform b_camera
{
	mat4 u_view;
	mat4 u_projection;
};

void main()
{
	texCoord = vec2(a_texCoord);
	gl_Position =  u_projection * u_view * a_vertexPosition;
}

#region Fragment

#version 440 core
			
layout(location = 0) out vec4 colour;

in vec2 texCoord;

uniform vec4 u_tint;
uniform sampler2D u_texData;

void main()
{
	colour = texture(u_texData , texCoord) * u_tint;
}