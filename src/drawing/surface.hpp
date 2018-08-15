#include <glad/glad.h>
#include <glm/glm.hpp>
#include "camera/camera.h"
#include "shader/shader.h"

class Surface
{
    public:
        float* vertices;
        unsigned int sizeofVertices;
        unsigned int* indices;
        unsigned int sizeofIndices;
        Camera* camera = NULL;
        mat4* perspective;
        Surface(unsigned int n, Camera* cam, mat4* persp)
        {
            vertices = new float[8*n*n];
            sizeofVertices = 8*n*n * sizeof(float);
            indices = new unsigned int[(n-1)*(n-1)*2*3];
            sizeofIndices =  (n-1)*(n-1)*2*3 * sizeof(unsigned int);
            InitVertices(n);
            InitIndices(n);
            InitVAO();
            camera = cam;
            perspective = persp;
        }
        ~Surface()
        {
            delete vertices;
            delete indices;
            //glDeleteVertexArrays(1, &VAO);
        }
        void Draw(mat4* model, Shader shader)
        {
            shader.use();
            //shader.setMat4("persp", *perspective);
            //shader.setMat4("view", camera->GetViewMatrix());
            shader.setMat4("model", *model);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, sizeofIndices / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        }
    private:
        unsigned int VAO, VBO, EBO;

        void InitVertices(unsigned int n)
        {
            if (n < 2)
            {
                std::cout << "ERROR::SURFACE:: n must be greater than 1" << std::endl;
                return;
            }
            float topLeftX = -1.0f;
            float topLeftY = -1.0f;
            float topRightX = 1.0f;
            float bottomLeftY = 1.0f;
            for(unsigned int i = 0; i < n; i++)
            {
                float t_x = (float)i / (float)(n-1);
                float positionX = t_x * topRightX + (1.0f - t_x) * topLeftX;
                for(unsigned int j = 0; j < n; j++)
                {
                    float t_z = (float)j / (float)(n-1);
                    float positionZ = t_z * bottomLeftY + (1.0f - t_z) * topLeftY;
                    unsigned int index = 8 * SquareToArray(i, j, n);
                    // Position
                    vertices[index] = positionX;
                    vertices[index+1] = 0;
                    vertices[index+2] = positionZ;
                    // Normal
                    vertices[index+3] = 0;
                    vertices[index+4] = 1.0f;
                    vertices[index+5] = 0;
                    // Texture coordinate
                    vertices[index+6] = (positionX - topLeftX) / 2.0f;
                    vertices[index+7] = (positionZ - bottomLeftY) / 2.0f;
                }
            }
        }
        void InitIndices(unsigned int n)
        {
            unsigned int index = 0;
            for(unsigned int i=0; i < n-1; i++)
            {
                for (unsigned int j=0; j < n-1; j++)
                {
                    unsigned int topLeftIndex = SquareToArray(i, j, n);
                    unsigned int topRightIndex = SquareToArray(i+1, j, n);
                    unsigned int bottomRightIndex = SquareToArray(i+1, j+1, n);
                    unsigned int bottomLeftIndex = SquareToArray(i, j+1, n);

                    indices[index] = topRightIndex;
                    indices[index+1] = topLeftIndex;
                    indices[index+2] = bottomLeftIndex;
                    indices[index+3] = topRightIndex;
                    indices[index+4] = bottomLeftIndex;
                    indices[index+5] = bottomRightIndex;
                    index += 6;
                }
            }
        }
        // ! Array is supposed to be filled column after column
        // ! i,j in [0, n-1]
        unsigned int SquareToArray(unsigned int i, unsigned int j, unsigned int n)
        {
            return i * n + j;
        }
        void InitVAO()
        {
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ARRAY_BUFFER, sizeofVertices, vertices, GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeofIndices, indices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            
            glBindVertexArray(0);
        }
};