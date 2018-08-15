#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    fragPos = vec3(model * vec4(aPos, 1.0));
    vec4 pos = proj * view * model * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
