#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices=6) out;

in VS_OUT {
    vec3 pos;
    vec3 color;
    vec3 normal;
    vec2 textCoord;
} gs_in[];

uniform mat4 persp;
uniform mat4 view;
uniform mat4 model;


vec3 computeNormal(int i)
{
    return gs_in[i].normal;
}

void drawNormal(vec4 origin, vec3 normal)
{
    vec4 ndc1 = persp * view * model * origin;
    gl_Position = ndc1;
    EmitVertex();
    vec4 ndc2 = persp * view * model * (origin + 0.2 * vec4(normal,0));
    gl_Position = ndc2;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    //draw(gl_in[0].gl_Position);
    vec3 normal0 = computeNormal(0);
    vec3 normal1 = computeNormal(1);
    vec3 normal2 = computeNormal(2);
    drawNormal(vec4(gs_in[0].pos, 1.0), normal0);
    drawNormal(vec4(gs_in[1].pos, 1.0), normal1);
    drawNormal(vec4(gs_in[2].pos, 1.0), normal2);
}