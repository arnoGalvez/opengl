#version 330 core

layout (location = 0) in vec2 NDC;

out vec3 fragPosViewSpace;
out vec2 TextCoord;

uniform mat4 perspInv;

void main()
{
    fragPosViewSpace = vec3(perspInv * vec4(NDC, 0.0, 1.0));
    TextCoord = NDC;
    gl_Position = vec4(NDC, 0, 1);
}
