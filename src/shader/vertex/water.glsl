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

uniform mat4 model;

const float OneOverSqrt2 = 0.7071;

vec4 ComputePosition(vec4 v);
vec3 ComputeNormal(vec4 v, float delta);
float rand(vec2 co);


void main()
{
    vec4 FragPos4 = ComputePosition(vec4(aPos, 1.0));
    gl_Position = persp * view * FragPos4;
    vs_out.FragPos = vec3(FragPos4);
    vs_out.Normal = ComputeNormal(vec4(aPos, 1.0), 0.00001);
    vs_out.TextCoord = aTextCoord;
    //gl_PointSize = max(4.0 - 2*gl_Position.z, 0.1);
}

vec4 ComputePosition(vec4 v)
{
    float period = 1.0 / D;
    vec4 pos = model * v;
    pos.y += H * sin(t + cos((pos.x * period)));
    pos.y += -H * cos(t + (pos.z * period));
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