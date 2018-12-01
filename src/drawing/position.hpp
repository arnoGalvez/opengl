#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace glm;

class Position
{
  public: // ANCHOR public
    Position(const unsigned int nX, const unsigned int nY, const unsigned int nZ, const float lX, const float lY, const float lZ)
    {
        pointsX = nX;
        pointsY = nY;
        pointsZ = nZ;
        lengthX = lX;
        lengthY = lY;
        lengthZ = lZ;
        AllocatePositions();
    }
    Position(const unsigned int n, const float length) : Position(n, n, n, length, length, length) {}
    ~Position()
    {
        delete positions;
    }

    mat4 *GetPtr()
    {
        return positions;
    }

    GLuint GetMatricesCount()
    {
        return pointsX * pointsY * pointsZ;
    }

    GLsizeiptr GetDataSize()
    {
        return GetMatricesCount() * sizeof(mat4);
    }

  private: // ANCHOR private
    unsigned int pointsX, pointsY, pointsZ;
    float lengthX, lengthY, lengthZ;
    mat4 *positions;
    void AllocatePositions()
    {
        positions = new mat4[pointsX * pointsY * pointsZ];
        float halfX = -lengthX / 2.0f;
        float halfY = -lengthY / 2.0f;
        float halfZ = -lengthZ / 2.0f;
        int verify = 0;
        for (size_t i = 0; i < pointsX; i++)
        {
            float tX = (float)i / (float)(pointsX == 1 ? 1 : pointsX - 1);
            float posX = halfX + tX * lengthX;
            for (size_t j = 0; j < pointsY; j++)
            {
                float tY = (float)j / (float)(pointsY == 1 ? 1 : pointsY - 1);
                float posY = halfY + tY * lengthY;
                for (size_t k = 0; k < pointsZ; k++)
                {
                    verify++;
                    float tZ = (float)k / (float)(pointsZ == 1 ? 1 : pointsZ - 1);
                    float posZ = halfZ + tZ * lengthY;
                    unsigned int index = GetArrayIndex(i, j, k);
                    mat4 pos;
                    pos = translate(pos, vec3(posX, posY, posZ));
                    for (size_t l = 0; l < 4; l++)
                    {
                        positions[index][l].x = pos[l].x;
                        positions[index][l].y = pos[l].y;
                        positions[index][l].z = pos[l].z;
                        positions[index][l].w = pos[l].w;
                    }
                }
            }
        }
    }
    unsigned int GetArrayIndex(int i, int j, int k)
    {
        return i + j * pointsX + k * pointsX * pointsY;
    }
};