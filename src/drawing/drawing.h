#ifndef __DRAWING_H
#define __DRAWING_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void DrawTriangle(unsigned int shaderProgram, unsigned int VAO);
void Triangle1(unsigned int *VAO, unsigned int *VBO, unsigned int *EBO, float *vertices, unsigned int *indices);
void Triangle2(unsigned int *VAO, unsigned int *VBO, unsigned int *EBO, float *vertices, unsigned int *indices);

#endif