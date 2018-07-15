#version 330 core
in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 FragColor;


struct Material {
    vec3 ambient;// the color reflected under ambient lighting
    sampler2D diffuse;// the color of the object under diffuse lighting
    sampler2D specular;// the color impact a specular light has on the object
    float shininess;// the scattering of specular highlight
    sampler2D emission;
};

struct Point {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float kC;
    float kL;
    float kQ;
};

struct Directional {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 ComputeDirectional(Directional light, Material mat, vec3 fragmentPos, vec2 textureCoord, vec3 norm, vec3 cameraPos);
vec3 ComputePoint(Point light, Material mat, vec3 fragmentPos, vec2 textureCoord, vec3 norm, vec3 cameraPos);
vec3 ComputeSpot(float spotAngle, vec3 spotCol, Material mat, vec2 textureCoord, vec3 fragmentPos, vec3 cameraPosition, vec3 cameraForward);

uniform Material material;

uniform Directional directional;

#define POINT_LIGHTS_COUNT 4
uniform Point pointLights[POINT_LIGHTS_COUNT];

uniform float spotAngleInner;
uniform vec3 spotColor;

uniform vec3 camPos;
uniform vec3 camForward;

void main()
{   
    vec3 directionalLight = ComputeDirectional(directional, material, fragPos, texCoord, normal, camPos);
    vec3 result = directionalLight;
    for (int i = 0; i < POINT_LIGHTS_COUNT; i++)
    {
        vec3 pointLight = ComputePoint(pointLights[i], material, fragPos, texCoord, normal, camPos);
        result += pointLight;
    }

    result += ComputeSpot(spotAngleInner, vec3(1), material, texCoord, fragPos, camPos, camForward);

    FragColor = vec4(result, 1.0);
}

vec3 ComputeDirectional(Directional light, Material mat, vec3 fragmentPos, vec2 textureCoord, vec3 norm, vec3 cameraPos) 
{
    vec3 normalizedLightDir = normalize(light.direction);
    vec3 normalizedNormal = normalize(norm);

    vec3 ambient = vec3(texture(mat.diffuse, textureCoord)) * light.ambient;

    float diffuseness = max(0, dot(- normalizedLightDir, normalizedNormal));
    vec3 diffuse = vec3(texture(mat.diffuse, textureCoord)) * (diffuseness * light.diffuse);

    float spec = 0;
    vec3 specular = vec3(0);
    if (diffuseness > 0)
    {
        vec3 reflectedRay = normalize( reflect(-normalizedLightDir, normalizedNormal));
        vec3 eyeDir = normalize(cameraPos - fragmentPos);
        spec = pow(max(0, dot(reflectedRay, eyeDir)), mat.shininess);
        specular = vec3(texture(mat.specular, textureCoord)) * (spec * light.specular);
    }

    return ambient + diffuse + specular;
}

vec3 ComputePoint(Point light, Material mat, vec3 fragmentPos, vec2 textureCoord, vec3 norm, vec3 cameraPos)
{
    vec3 normalizedLightDir = normalize(fragmentPos - light.position);
    vec3 normalizedNormal = normalize(norm);

    vec3 ambient = vec3(texture(mat.diffuse, textureCoord)) * light.ambient;

    float diffuseness = max(0, dot(- normalizedLightDir, normalizedNormal));
    float attenuation = 1;
    if (diffuseness > 0)
    {
        float  d = distance(light.position, fragmentPos);
        attenuation = 1.0 / (light.kC + (light.kL + light.kQ * d) * d );
    }
    vec3 diffuse = vec3(texture(mat.diffuse, textureCoord)) * (diffuseness * light.diffuse);

    float spec = 0;
    vec3 specular = vec3(0);
    if (diffuseness > 0)
    {
        vec3 reflectedRay = normalize( reflect(-normalizedLightDir, normalizedNormal));
        vec3 eyeDir = normalize(cameraPos - fragmentPos);
        spec = pow(max(0, dot(reflectedRay, eyeDir)), mat.shininess);
        specular = vec3(texture(mat.specular, textureCoord)) * (spec * light.specular);
    }

    return (ambient + diffuse + specular) * attenuation;
}

vec3 ComputeSpot(float spotAngle, vec3 spotCol, Material mat, vec2 textureCoord, vec3 fragmentPos, vec3 cameraPosition, vec3 cameraForward)
{
    vec3 dir = normalize(fragmentPos - cameraPosition);
    float angle = dot (dir, normalize(cameraForward));
    float spot = 0;
    if (angle > spotAngle)
    {   
        spot = (angle - spotAngle) / (1.0 - spotAngle);
    }

    return spot * vec3(texture(mat.diffuse, textureCoord));
}
