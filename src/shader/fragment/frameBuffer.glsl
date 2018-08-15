#version 330 core
in vec2 textCoord;

out vec4 FragColor;

uniform sampler2D screen;

const float offset = 1.0 / 300.0;

vec2 neighbourhood[9] = vec2[](
    vec2(-offset, offset), vec2(0, offset), vec2(offset, offset),
    vec2(-offset, 0), vec2(0, 0), vec2(0, offset),
    vec2(-offset, -offset), vec2(0, -offset), vec2(offset, -offset)
);

/*float kernel[9] = float[](
    1, 3, 1,
    3, 4, 3,
    1, 3, 1
);//*/

float kernel[9] = float[](
    0, 0, 0,
    0, 1, 0,
    0, 0, 0
);


void main()
{
    vec3 color = vec3(0.0);
    for (int i=0; i < 9; i++)
    {
        color += 1.0 * kernel[i] * vec3(texture(screen, textCoord +  neighbourhood[i]));
    }
    FragColor = vec4(color, 1.0);
}