#version 330 core   
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D textureDiffuse0;

uniform sampler2D textureSpecular0;

void main()
{
    fragColor = texture(textureDiffuse0, texCoord);
}