#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform float xSpeed;
uniform float ySpeed;
uniform float r;
uniform float g;
uniform float b;
out vec4 theColor;
out vec2 textCoord;

void main()
{
	gl_Position = vec4(aPos.x + xSpeed, aPos.y + ySpeed, aPos.z, 1.0);
	theColor = vec4(r, g, b, aColor.w);
	textCoord = aTexCoord;
}