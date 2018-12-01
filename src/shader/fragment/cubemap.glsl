#version 330 core

in vec3 fragPos;
out vec4 fragColor;
uniform samplerCube skybox;

void main()
{
    fragColor = texture(skybox, fragPos);
    //fragColor = vec4(normalize(fragPos) * 0.5 + 0.5,1);
}