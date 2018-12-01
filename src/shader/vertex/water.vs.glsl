#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCoord;

out VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TextCoord;
} vs_out;

layout (std140) uniform Matrices
{
    uniform mat4 persp;
    uniform mat4 view;
};

layout (std140) uniform Wave
{
    uniform float t;
    uniform float D;
    uniform float H;
};
struct Noise {
    sampler2D texture;
    int width;
    int height;
    float frequency;
    float amplitude;
};

uniform Noise noise[8];
uniform mat4 model;

const float OneOverSqrt2 = 0.7071;
const float pi = 3.14159265359;

vec4 ComputePosition(vec4 v);
vec3 ComputeNormal(vec4 v, float delta);
float rand(vec2 co);
float EvalNoise(Noise n, vec2 pos);
float Interpolate(float a, float b, float t);

void main()
{
    vec4 FragPos4 = ComputePosition(vec4(aPos, 1.0));
    gl_Position = persp * view * FragPos4;
    vs_out.FragPos = vec3(FragPos4);
    vs_out.Normal = ComputeNormal(vec4(aPos, 1.0), 0.00001);
    vs_out.TextCoord = aTextCoord + vec2(1);
    //gl_PointSize = max(4.0 - 2*gl_Position.z, 0.1);
}

vec4 ComputePosition(vec4 v)
{
    // float period = 1.0 / D;
    vec4 pos = model * v;
    // pos.y += H * sin(t + cos((pos.x * period)));
    // pos.y += -H * cos(t + (pos.z * period));
    float sum = 0;
    for (int i = 0; i < 4; i++)
    {
        sum += EvalNoise(noise[i], vec2(v.xz) + vec2(t*0.1));
    }

    pos.y = sum * H;
    return pos;
}

vec3 ComputeNormal(vec4 v, float delta)
{
    vec3 p1 = vec3(ComputePosition(v + vec4(delta, 0.0, 0.0, 1.0)));
    vec3 p2 = vec3(ComputePosition(v + OneOverSqrt2 * vec4(-delta, 0.0, -delta, 1.0)));
    vec3 p3 = vec3(ComputePosition(v + OneOverSqrt2 * vec4(-delta, 0.0, delta, 1.0)));
    vec3 normal = cross(p3 - p2, p1 - p2);
    //vec3 normal = cross(p1 - p2, p3 - p2);

    return normal;
}

float rand(vec2 co)
{
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float Interpolate(float a, float b, float t)
{
    // return (1 - t)*a + t*b;
    float tmp = -t * pi;
    tmp = (1 - cos(tmp)) * 0.5;
    return (1 -tmp)*a + tmp*b;
}

float EvalNoise(Noise n, vec2 pos)
{
    vec2 point = n.frequency * pos;

    float invWidth = (1.0 / float(n.width));
    float invHeight = (1.0 / float(n.height));

    float c00_x = (float(int(point.x * n.width)) / float(n.width)) + invWidth;
    float c11_x = c00_x + invWidth;
    float c00_y = (float(int(point.y * n.height)) / float(n.height)) + invHeight;
    float c11_y = c00_y + invHeight;
    vec2 c00 = vec2(c00_x, c00_y);
    vec2 c11 = vec2(c11_x, c11_y);
    vec2 c01 = c00 + vec2(0.0, invHeight);
    vec2 c10 = c00 + vec2(invWidth, 0.0);

    float t_x = (point.x - (c00_x - invWidth)) * n.width;
    float t_y = (point.y - (c00_y - invHeight)) * n.height;
    float a = Interpolate(texture(n.texture, c00).r, texture(n.texture, c10).r, t_x);
    float b = Interpolate(texture(n.texture, c01).r, texture(n.texture, c11).r, t_x);
    float noiseValue = Interpolate(a, b, t_y);
    return noiseValue * n.amplitude;
}
