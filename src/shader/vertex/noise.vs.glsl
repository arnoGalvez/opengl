#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCoord;

out VS_OUT {
    vec3 pos;
    vec3 color;
    vec3 normal;
    vec2 textCoord;
} vs_out;

uniform mat4 persp;
uniform mat4 view;
uniform mat4 model;

layout (std140) uniform Matrices {
    mat4 positions[100];
};

void main()
{
    vs_out.pos = aPos;
    vs_out.color = vec3(aTextCoord, 0.1);
    vs_out.normal = aNormal;
    vs_out.textCoord = aTextCoord;
    gl_Position = persp * view * positions[gl_InstanceID] * vec4(aPos, 1.0);
}