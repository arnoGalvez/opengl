#version 330 core

in VS_OUT {
    vec3 pos;
    vec3 color;
    vec3 normal;
    vec2 textCoord;
} fs_in;

out vec4 fragColor;

const float pi = 3.14159265359;

// SECTION structs
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

// SECTION uniforms
uniform Spiral spiral;
uniform Noise noise[8];
uniform int INDEX;
layout (std140) uniform Arrays {
    int permutationTable[512];
};
uniform float time;
uniform int table[512];
// !SECTION 
/**
 * Given a spiral, an angle and a positive int k, returns t = (angle / spiral.angularSpeed) + 2*k*pi / spiral.angularSpeed
*/
float FindParameterValue(Spiral sp, float angle, int k);
/**
 * Given a spiral and a point, finds the distance between that point and the spiral
*/
float DistanceToSpiral(Spiral sp, vec2 point);
/**
 * Returns the angle between v1 and (1,0), in radians
*/
float Angle(vec2 v1);

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

float Floor(float x)
{
    return float(int(x));
}

int TextCoordToIndex(int noiseWidth, float x)
{
    return int(noiseWidth * (x - Floor(x)));
}

float IndexToTextCoord(int tableWidth, int i)
{
    float coord = float(i) / float(tableWidth-1);
    return coord;
} 

float EvalNoise(Noise n, vec2 pos)
{
    vec2 point = n.frequency * (pos - vec2(Floor(pos.x), Floor(pos.y)));

    float invWidth = (1.0 / float(n.width));
    float invHeight = (1.0 / float(n.height));

    float c00_x = (float(int(point.x * n.width)) / float(n.width));
    float c11_x = c00_x + invWidth;
    float c00_y = (float(int(point.y * n.height)) / float(n.height));
    float c11_y = c00_y + invHeight;
    vec2 c00 = vec2(c00_x, c00_y);
    vec2 c11 = vec2(c11_x, c11_y);
    vec2 c01 = c00 + vec2(0.0, invHeight);
    vec2 c10 = c00 + vec2(invWidth, 0.0);

    float t_x = (point.x - c00_x) * n.width;
    float t_y = (point.y - c00_y) * n.height;
    float noiseValue00 = texture(n.texture, c00).r;
    float noiseValue10 = texture(n.texture, c10).r;
    float noiseValue01 = texture(n.texture, c01).r;
    float noiseValue11 = texture(n.texture, c11).r;
    float a = Interpolate(noiseValue00, noiseValue10, t_x);
    float b = Interpolate(noiseValue01, noiseValue11, t_x);
    float noiseValue = Interpolate(a, b, t_y);
    return noiseValue * n.amplitude;
    

    // Perlin noise
    //-------------
    /*float x0 = float(int(point.x * n.width)) * invWidth;
    float y0 = float(int(point.y * n.width)) * invHeight;
    float x1 = x0 + invWidth;
    float y1 = y0 + invHeight;
    int c00_x = TextCoordToIndex(n.width, x0);
    int c00_y = TextCoordToIndex(n.width, y0);
    int c11_x = TextCoordToIndex(n.width, x1);
    int c11_y = TextCoordToIndex(n.width, y1);
    float c00_v = texture(n.texture, IndexToTextCoord(n.width, permutationTable[permutationTable[c00_x] + c00_y])).r;
    float c01_v = texture(n.texture, IndexToTextCoord(n.width, permutationTable[permutationTable[c00_x] + c11_y])).r;
    float c11_v = texture(n.texture, IndexToTextCoord(n.width, permutationTable[permutationTable[c11_x] + c11_y])).r;
    float c10_v = texture(n.texture, IndexToTextCoord(n.width, permutationTable[permutationTable[c11_x] + c00_y])).r;
    float t_x = (point.x - x0) * n.width;
    float t_y = (point.y - y0) * n.width;
    float a = Interpolate(c00_v, c10_v, t_x);
    float b = Interpolate(c01_v, c11_v, t_x);
    float noiseValue = Interpolate(a, b, t_y) * n.amplitude;

    return noiseValue;//*/
}

float SinTransform(float noiseValue)
{
    return abs(sin(30*fs_in.textCoord.y + 0.2*noiseValue)); // ANCHOR sinus modulation
}

float TurbuTransform(float noiseValue)
{
    return abs(2*noiseValue - 1);
}

float WoodTransform(float noiseValue)
{
    int g = 4;
    float tmp = noiseValue * g;
    return tmp - float(int(tmp));
}

void main()
{
    // spiral
    //-------
    // vec2 point = fs_in.textCoord - vec2(0.5, 0.5);
    // float d = DistanceToSpiral(spiral, point);
    // vec4 background = vec4(1,0.1,0.1,0);
    // vec4 foreground = vec4(0,0,0,1);
    // fragColor = mix(foreground, background, d * 100);
    // fragCsolor = vec4(Angle(point) / pi);
    
    float sum = 0;
    for (int i = 0; i < 4; i++)
    {
        sum += EvalNoise(noise[i], fs_in.textCoord + vec2(time));
    }
    // sum = SinTransform(sum);
    sum = TurbuTransform(sum);
    // sum = WoodTransform(sum);
    fragColor = vec4(clamp(sum, 0, 1));
    
    // fragColor = vec4(clamp(permutationTable[TextCoordToIndex(255, fs_in.textCoord.x)], 0, 1));
    // fragColor = vec4(clamp(float(permutationTable[INDEX]), 0, 1));
    // fragColor = vec4(clamp(float(table[INDEX]), 0, 1));
    // fragColor = vec4(Floor(fs_in.textCoord.x));
    // fragColor = texture(noise[0].texture, fs_in.textCoord.x);
}