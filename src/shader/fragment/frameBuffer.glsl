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

const float kernel0[9] = float[](
    0, 0, 0,
    0, 1, 0,
    0, 0, 0
);

const float kernel1[9] = float[](
    1, 2, 1,
    2, 4, 2,
    1, 2, 1
);

const float kernel2[9] = float[](
    -1, -1, -1,
    -1, 8, -1,
    -1, -1, -1
);

const float kernel3[9] = float[](
    1, 0, -1,
    0, 0, 0,
    -1, 0, 1
);


void main()
{
    vec3 color = vec3(0.0);
    for (int i=0; i < 9; i++)
    {
        color += kernel0[i] * vec3(texture(screen, textCoord +  neighbourhood[i]));
    }
    FragColor = vec4(color, 1.0);
}