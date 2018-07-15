#version 330 core
out vec4 fragColor;

in vec2 color;

uniform sampler2D T;

void main()
{
	fragColor = texture(T, color);
}