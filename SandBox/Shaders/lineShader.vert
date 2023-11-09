#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform float color;
uniform mat4 projection;

out vec4 vertexColor;

void main()
{
	vertexColor = vec4(color,0,0,0);
	gl_Position = projection * model * vec4(aPos, 1.0);
}