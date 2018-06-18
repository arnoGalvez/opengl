#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "textures/import.h"
#include "window/window.h"

#define BUFFER_OFFSET(offset) ((void*) (offset))
#define STRIDE(n, type) (n * (sizeof(type)))


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Halo Infinite", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Shader shaderProgramTexture("/home/arno/opengl/src/shader/vertexTextureShader.glsl", "/home/arno/opengl/src/shader/fragTextureShader.glsl");
    /*float vertices[] = {
        //positions         //colors                   
        0.5f, 0.5f,    1.f, 1.f, // top right
        0.5, -0.5f,   1.f, 0.f,  // bottom right
        -0.5f, -0.5f,     0.f, 0.f,// bottom left
        -0.5f, 0.5f,     0.f, 1.f// top left
    };

    unsigned int indices[] = {
        0,1,3,1,2,3
    };

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    //5: we tell the vertex attrib array to activate a third attrib
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIDE(9, GL_FLOAT), BUFFER_OFFSET(7 * sizeof(GL_FLOAT)));
    //glEnableVertexAttribArray(2);    

    GenTexture2D image1("/home/arno/opengl/src/textures/halo-infinite.jpg", GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
    //GenTexture2D image2("/home/arno/opengl/src/textures/philip.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
    
    glBindVertexArray(0);//*/
    
    int n = 50;
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
        std::cout << "vertex: " << circleVertices[j] << " " << circleVertices[j+1] << std::endl;
        circleIndices[3*i] = 0;
        circleIndices[3*i + 1] = i+1;
        circleIndices[3*i + 2] = ((i == (n - 1)) ? 1 : i+2);
        std::cout << circleIndices[3*i + 1] << " " << circleIndices[3*i+2] << std::endl;
    }
    

    unsigned int VAO2, VBO2, EBO2;

    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(circleIndices), circleIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, STRIDE(4, float), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, STRIDE(4, float), BUFFER_OFFSET(2*sizeof(float)));
    
    GenTexture2D image3("/home/arno/opengl/src/textures/halo-infinite.jpg", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST);
    glBindVertexArray(0);//*/
    
    
    shaderProgramTexture.use();
    //shaderProgramTexture.setFloat("t", 2.f);
    glUniform1i(glGetUniformLocation(shaderProgramTexture.ID, "T"), 0);
    /*glUniform1i(glGetUniformLocation(shaderProgramTexture.ID, "text2"), 1);
    unsigned int mLoc = glGetUniformLocation(shaderProgramTexture.ID, "m");//*/


    glClearColor(0.f, 3.f, 0.f, 1.f);
    

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        process_input(window);
        glClear(GL_COLOR_BUFFER_BIT);
        /*glm::mat4 m1;
        m1 = glm::rotate(m1, (float)(cos(glfwGetTime())), glm::vec3(0, 0, 1));
        glm::mat4 m2;
        m2 = glm::translate(m2, glm::vec3(1, 0, 0));
        glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(m1));
        shaderProgramTexture.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, image3.textID);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, image2.textID);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(m2));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//*/
        shaderProgramTexture.use();
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, image3.textID);
        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 3*n, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }//*/


    glfwTerminate();
    return 0;
}

