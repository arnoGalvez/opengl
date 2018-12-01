#version 330 core   

in vec3 fragPos;
in vec2 texCoord;
in vec3 normal;
out vec4 fragColor;
uniform sampler2D textureDiffuse0;
uniform sampler2D textureDiffuse1;
uniform sampler2D textureDiffuse2;

uniform sampler2D textureSpecular0;
uniform sampler2D textureSpecular1;
uniform sampler2D textureSpecular2;

uniform sampler2D textureAmbient0;
uniform sampler2D textureAmbient1;
uniform sampler2D textureAmbient2;

uniform samplerCube skybox;
uniform vec3 camPos;

void main()
{
    vec3 diffuse = vec3(texture(textureDiffuse0, texCoord));
    vec3 reflectionVect = reflect(fragPos - camPos, normalize(normal));
    //vec3 reflectColor = vec3(texture(skybox, reflectionVect) * texture(textureAmbient0, texCoord));
    //fragColor = vec4(diffuse + reflectColor, 1.0);
    fragColor = vec4(diffuse, 1.0);

}