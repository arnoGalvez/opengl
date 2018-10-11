#version 330 core

in VS_OUT {
    vec3 pos;
    vec3 color;
    vec3 normal;
    vec2 textCoord;
} fs_in;

out vec4 fragColor;

const float pi = 3.14159265359;

// SECTION struct
/**
 * Describes a spiral of the following form : amplitudeSpeed * t * exp(i * angularSpeed * t)
*/
struct Spiral {
    float amplitudeSpeed;
    float angularSpeed;
};

/**
 * Describes a noise texture
*/
struct Noise {
    sampler2D texture;
    int width;
    int height;
    float frequency;
    float amplitude;
};

// !SECTION

// SECTION uniform
uniform Spiral spiral;
uniform Noise noise[8];
uniform float time;
// !SECTION 
/**
 * Returns the angle between v1 and (1,0), in radians
*/
float Angle(vec2 v1);
/**
 * Given a spiral, an angle and a positive int k, returns t = (angle / spiral.angularSpeed) + 2*k*pi / spiral.angularSpeed
*/
float FindParameterValue(Spiral sp, float angle, int k);
/**
 * Given a spiral and a point, finds the distance between that point and the spiral
*/
float DistanceToSpiral(Spiral sp, vec2 point);

// Functions
//----------

float FindParameterValue(Spiral sp, float angle, int k)
{
    return (angle + 2*k*pi) / sp.angularSpeed;
}

float DistanceToSpiral(Spiral sp, vec2 point)
{
    float norm = length(point);
    float angle = Angle(point);
    int k = 0;
    float minor = FindParameterValue(sp, angle, k);
    float major = FindParameterValue(sp, angle, k+1);
    while(!(sp.amplitudeSpeed * minor <= norm && norm < sp.amplitudeSpeed * major) && k < 25)
    {
        ++k;
        minor = FindParameterValue(sp, angle, k);
        major = FindParameterValue(sp, angle, k+1);
    }
    float d = min(norm - minor*sp.amplitudeSpeed, major*sp.amplitudeSpeed - norm);
    return d; 
}

float Angle(vec2 v1)
{
    vec2 v = normalize(v1);
    float dot1 = dot(v, vec2(1,0));
    float dot2 = dot(v, vec2(0,-1));
    float factor = (-dot1 + 1) / 2.0;
    if (dot2 < 0)
    {
        return factor * pi;
    }
    else
    {
        return (2 - factor) * pi;
    }
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


void main()
{
    // vec2 point = fs_in.textCoord - vec2(0.5, 0.5);
    // float d = DistanceToSpiral(spiral, point);
    // vec4 background = vec4(1,0.1,0.1,0);
    // vec4 foreground = vec4(0,0,0,1);
    // fragColor = mix(foreground, background, d * 100);
    // fragCsolor = vec4(Angle(point) / pi);
    
    // fragColor = vec4(texture(noise.texture, fs_in.textCoord).x);
    float sum = 0;
    for (int i = 0; i < 4; i++)
    {
        sum += EvalNoise(noise[i], fs_in.textCoord);
    }
    // fragColor = vec4(EvalNoise(noise, fs_in.textCoord + vec2(time * 0.4)));
    fragColor = vec4(sum);
}