#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 textCoord;
//layout (location = 1) in vec2 textCoord;

out vec2 color;

void main()
{
	gl_Position = vec4(pos.x, pos.y, 0.0, 1.0); 
	color = textCoord;
}
