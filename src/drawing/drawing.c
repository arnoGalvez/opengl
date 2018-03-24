#include "drawing.h"

void DrawTriangle(unsigned int shaderProgram, unsigned int VAO)
{
    glad_glUseProgram(shaderProgram);
    glad_glBindVertexArray(VAO);
    glad_glDrawArrays(GL_TRIANGLES, 0, 3);
    glad_glBindVertexArray(0);
}

void Triangle1(unsigned int *VAO, unsigned int *VBO, unsigned int *EBO, float *vertices, unsigned int *indices)
{
    glGenVertexArrays(1, VAO);// number of vertex array objects to be created -- array of identifiers
    glGenBuffers(1, VBO);// number of vertex buffer objects to be created -- array of identifiers
    glad_glGenBuffers(1, EBO);
    
    glad_glBindVertexArray(*VAO);

    // 0. copy vertices into the buffer VBO
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//4th param : the data will most likely not change at all or very rarely

    // 1. copy index array into the buffer EBO
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 2. How the vertex data should be treated / vertex attribute
    glad_glEnableVertexAttribArray(0);// enable the attribute at location 0
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);

    // the call to glVertexAttribPointer bound VBO to be the vertex buffer object asociated to those certex attributes
    // so we can unbind VBO
    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);


    glad_glBindVertexArray(0);
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Triangle2(unsigned int *VAO, unsigned int *VBO, unsigned int *EBO, float *vertices, unsigned int *indices)
{
    glGenVertexArrays(1, VAO);// number of vertex array objects to be created -- array of identifiers
    glGenBuffers(1, VBO);// number of vertex buffer objects to be created -- array of identifiers
    glad_glGenBuffers(1, EBO);
    
    glad_glBindVertexArray(*VAO);

    // 0. copy vertices into the buffer VBO
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) - 6 * sizeof(float), vertices + 6, GL_STATIC_DRAW);//4th param : the data will most likely not change at all or very rarely

    // 1. copy index array into the buffer EBO
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) - 3 * sizeof(unsigned int), indices + 3, GL_STATIC_DRAW);

    // 2. How the vertex data should be treated / vertex attribute
    glad_glEnableVertexAttribArray(0);// enable the attribute at location 0
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);

    // the call to glVertexAttribPointer bound VBO to be the vertex buffer object asociated to those certex attributes
    // so we can unbind VBO
    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);


    glad_glBindVertexArray(0);
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}