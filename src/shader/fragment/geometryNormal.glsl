#version 330 core

in VS_OUT {
    vec3 pos;
    vec3 color;
    vec3 normal;
} fs_in;

out vec4 fragColor;

uniform vec3 color;

void main()
{
    fragColor = vec4(1,0,0,1);
}