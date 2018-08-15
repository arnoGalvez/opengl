#version 330 core

in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D image;

void main()
{
    fragColor = texture(image, texCoord);
    if (fragColor.a < 0.1)
        discard;
}