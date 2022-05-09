#region Vertex

#version 440 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2  aTexCoords;

out vec2 texCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	texCoord = vec2(aTexCoords);
	gl_Position = vec4(aPos, 1.0);
}

#region Fragment

#version 440 core

layout(location = 0) out vec4 colour;

in vec2 texCoord;

uniform sampler2D image;
uniform sampler2D image2;

void main()
{
	vec3 col1 = texture(image, texCoord).rgb;
	vec3 col2 = texture(image2, texCoord).rgb;
	image1 += image2;
	colour = vec4(image1, 1.0);
}