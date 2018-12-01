#version 330 core

in vec3 fragPosViewSpace;
in vec2 TextCoord;

out vec4 FragColor;

struct Ray// in view space
{
    vec3 origin;
    vec3 direction;
};
// types:
// 0 : Lambertian
// 1 : metal
struct Material
{
    vec3 color;
    int type;
    float fuzz;
};

struct Sphere// in view space
{
    vec4 center;
    float radius;
    Material material;
};

struct HitRecord// in view space
{
    float t;// the t for which ray.origin + t * ray.direction = point on a sphere
    vec3 point;// the hitpoint on the sphere
    vec3 normal;// the normal to that point on the sphere 
    Material material;
};

vec3 RandomPointOnUnitSphere();


// * Variables
// * ---------
uniform Sphere spheres[3];
uniform int spheresCount;
uniform sampler2D Noise;
HitRecord record;


vec3 GetPoint(Ray ray, float t)
{
    return ray.origin + t * ray.direction;
}

void ChangeRecordMaterial(Material mat)
{
    record.material.color = mat.color;
    record.material.type = mat.type;
    record.material.fuzz = mat.fuzz;
}

vec3 ApplyMaterial(Material material)
{
    return material.color;
}

// uses current record and a material to determine the direction of the next ray
Ray ScatterRayWithMat(Material mat, Ray rayIn)
{
    Ray rayOut;
    rayOut.direction = vec3(0);
    if (mat.type == 0)
    {
        rayOut.origin = record.point;
        rayOut.direction = (RandomPointOnUnitSphere() + record.normal);
        return rayOut;
    }
    if (mat.type == 1)
    {
        rayOut.origin = record.point;
        vec3 reflectedRayDir = reflect(rayIn.direction, record.normal);
        vec3 randCorrection = mat.fuzz * RandomPointOnUnitSphere();
        reflectedRayDir += randCorrection;
        if (dot(reflectedRayDir, record.normal) > 0)
            rayOut.direction = reflectedRayDir;
        return rayOut;
    }
    return rayOut;
}

bool Hit(Ray ray, Sphere sphere, float t_min, float t_max)
{
    vec3 oc = ray.origin - sphere.center.xyz;
    float a = dot(ray.direction, ray.direction);
    float b = 2.0 * dot(ray.direction, oc);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b*b - 4*a*c;
    if (discriminant > 0 && a != 0)
    {
        //FragColor = vec4(0.5, 0.0, 0.0, 1.0);
        float sqrtDiscri = sqrt(discriminant);
        float t1 = (-b - sqrtDiscri) / (2 * a);
        if (t_min < t1 && t1 < t_max)
        {
            record.t = t1;
            record.point = GetPoint(ray, t1);
            record.normal = (record.point - sphere.center.xyz) / sphere.radius;// IN VIEW SPACE
            //record.normal = normalize(vec3(discriminant));
            ChangeRecordMaterial(sphere.material);
            return true;
        }
        float t2 = (-b - sqrtDiscri) / (2 * a);
        if (t_min < t2 && t2 < t_max)
        {
            record.t = t2;
            record.point = GetPoint(ray, t2);
            record.normal = (record.point - sphere.center.xyz) / sphere.radius;// IN VIEW SPACE
            //record.normal = normalize(vec3(discriminant));

            ChangeRecordMaterial(sphere.material);
            return true;
        }
    }       

    return false;
}

bool SphereHit(Ray ray, float t_min, float t_max)
{
    if (ray.direction == vec3(0))
        return false;
    bool anythingHit = false;
    float closestSoFar = t_max;
    for (int i=0; i < spheresCount; i++)
    {
        if (Hit(ray, spheres[i], t_min, closestSoFar))
        {
            anythingHit = true;
            closestSoFar = record.t;
        }
    }
    return anythingHit;
}
vec3 shift;
void InitRand()
{
    shift = vec3(1);
}
vec3 RandomPointOnUnitSphere()
{
    float x = fract(sin(dot(TextCoord, vec2(12.9898, 4.1414))) * 43758.5453);
    float y = fract(sin(dot(TextCoord, vec2(12.9898, 4.1414))) * 43758.5453);
    float z = fract(sin(dot(TextCoord, vec2(12.9898, 4.1414))) * 43758.5453);
    shift += vec3(x,y,z);
    return (vec3(x,y,z));//*/
    /*vec4 texel = texture(Noise, TextCoord + shift);
    shift = vec2(texel.xy + texel.yz);
    return ((texel.xyz - vec3(0.5)) * 1.0);//*/
}

void main()
{
    InitRand();
    Ray ray;
    vec3 color = vec3(0);
    for (int i = 0; i < 20; i ++)
    {
        ray.origin = vec3(0);
        ray.direction = normalize(fragPosViewSpace) + RandomPointOnUnitSphere()*0.002;
        int bounces = 0;

        vec3 rayColor = vec3(1);

        if (SphereHit(ray, 0.001, 99999.0))
        {
            bounces += 1;
            ray = ScatterRayWithMat(record.material, ray);
            rayColor *= ApplyMaterial(record.material);
            bool hasHit = SphereHit(ray, 0.001, 99999.0);
            while(hasHit && bounces < 50)
            {
                bounces += 1;
                ray = ScatterRayWithMat(record.material, ray);
                rayColor *= ApplyMaterial(record.material); 
                hasHit = SphereHit(ray, 0.001, 99999.0);
            }
           color += rayColor;
        }
        else if (i == 0)
            discard;
    }
    vec3 average = color / 20.0;
    average = vec3(pow(average.x, 0.5), pow(average.y, 0.5), pow(average.z, 0.5));
    FragColor = vec4(average, 1.0);
    
}