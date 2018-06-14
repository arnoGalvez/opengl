#version 330 core
out vec4 fragColor;

in vec4 theColor;
in vec2 textCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
	fragColor = mix(texture(texture1, textCoord), texture(texture2, vec2(1. - textCoord.x, 1. - textCoord.y)), 0.5);
}
