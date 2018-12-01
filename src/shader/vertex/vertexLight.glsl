#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCoord;

out vec3 Pos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Pos = vec3(model * vec4(pos, 1.0));
    gl_Position = proj * view * model * vec4(pos, 1.0);
}