#version 330 core

in vec3 Pos;
in vec3 Normal;
out vec4 FragColor;

uniform vec3 lightColor;
uniform samplerCube skybox;
uniform vec3 camPos;

void main()
{
    vec3 ray = refract(normalize(Pos - camPos), normalize(Normal), 1.0/1.52);
    FragColor = texture(skybox, ray);
}
