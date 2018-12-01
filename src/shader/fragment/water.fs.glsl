#version 330 core
in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TextCoord;
} fs_in;

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirectionalLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 direction;
};

vec3 ComputeDirectionnalLight(DirectionalLight light, Material mat);

layout (std140) uniform Camera
{
    vec3 camPos;
};
//uniform vec3 camPos;
uniform Material material;
uniform DirectionalLight diretionalLight;

void main()
{
    FragColor = vec4(ComputeDirectionnalLight(diretionalLight, material), 0.7);
    // FragColor = vec4(normalize(fs_in.Normal) * 0.5 + 0.5, 1.0);
}

vec3 ComputeDirectionnalLight(DirectionalLight light, Material mat)
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular = vec3(0);
    vec3 normNormal = normalize(fs_in.Normal);

    ambient = light.ambient * mat.ambient;

    vec3 normLightDir = normalize(light.direction);
    float diffuseFactor = max(0, dot(normNormal, -normLightDir));
    diffuse = diffuseFactor * light.diffuse * mat.ambient;
    
    vec3 reflectedRay = reflect(normLightDir, normNormal);
    vec3 normFragCam = normalize(camPos - fs_in.FragPos);
    float specularFactor = pow(max(0, dot(reflectedRay, normFragCam)), mat.shininess);
    specular = specularFactor * light.diffuse * mat.specular;
    
    return ambient + diffuse + specular;
}
