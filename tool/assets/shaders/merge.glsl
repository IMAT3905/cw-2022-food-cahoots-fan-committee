#region Vertex

#version 440 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2  aTexCoords;

out vec2 texCoord;

void main()
{
	texCoord = vec2(aTexCoords);
	gl_Position = vec4(aPos, 1.0);
}

#region Fragment

#version 440 core

layout(location = 0) out vec4 colour;

in vec2 texCoord;

uniform sampler2D image1;
uniform sampler2D image2;

void main()
{
	vec4 col1 = texture(image1, texCoord);
	vec4 col2 = texture(image2, texCoord);
	col1 *= col2;
	colour = vec4(col1);
}