#version 330 core   
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D textureDiffuse0;

void main()
{
    fragColor = vec4(1, 1, 1, 1);
}