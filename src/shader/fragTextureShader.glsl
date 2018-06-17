#version 330 core
out vec4 fragColor;

in vec4 vexColor;
in vec2 vexTextCoord;

uniform float t;
uniform sampler2D text1;
uniform sampler2D text2;

void main()
{
	fragColor = mix(texture(text1, vexTextCoord), texture(text2, vexTextCoord * t), 0.5) * vexColor;
}