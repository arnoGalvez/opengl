#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 textCoord;
//layout (location = 1) in vec2 textCoord;

out vec2 color;
uniform float xAdjust;
uniform float yAdjust;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(pos.x * xAdjust, pos.y * yAdjust, 0.0, 1.0); 
	color = textCoord;
}
