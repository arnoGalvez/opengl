#include <iostream>
#include <cmath>
class circle
{
    public:
        int n;
        float radius;
        float *circleVertices;
        circle(int verticesCount, float r)
        {
            n = verticesCount;
            radius = r;
            circleVertices = new float[2*(n + 1)];
            circleVertices[0] = 0.f;
            circleVertices[1] = 0.f;
            circleVertices[2] = 0.5f;
            circleVertices[3] = 0.5f;
            for(int i = 1; i < n + 1; i++)
            {
                int j = 2*i;
                float temp = ((float)i / n) * 2 * 3.14f;
                circleVertices[j] = cos(temp) * radius;
                circleVertices[j+1] = sin(temp) * radius;
            }
        }
        ~circle()
        {
            delete circleVertices;
        }
};

/*int n = 200;
    float circleVertices[4*(n + 1)];
    unsigned int circleIndices[3*n];
    circleVertices[0] = 0.f;
    circleVertices[1] = 0.f;
    circleVertices[2] = 0.5f;
    circleVertices[3] = 0.5f;
    for(int i = 0; i < n; i++)
    {
        int j = 4*i + 4;
        float temp = ((float)i / n) * 2 * M_PI;
        circleVertices[j] = cos(temp) / 2.f;
        circleVertices[j+1] = sin(temp) / 2.f;
        circleVertices[j+2] = circleVertices[j] + 0.5f;
        circleVertices[j+3] = circleVertices[j+1] + 0.5f;
        //std::cout << "vertex: " << circleVertices[j] << " " << circleVertices[j+1] << std::endl;
        circleIndices[3*i] = 0;
        circleIndices[3*i + 1] = i+1;
        circleIndices[3*i + 2] = ((i == (n - 1)) ? 1 : i+2);
        //std::cout << circleIndices[3*i + 1] << " " << circleIndices[3*i+2] << std::endl;
    }*/