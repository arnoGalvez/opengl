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

using namespace glm;

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
    
    int n = 200;
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
    
    glBindVertexArray(0);//*/

    float cube[] = {
        0.f, 0.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f, 1.f,
        1.f, 1.f, 0.f, 1.f, 1.f,
        1.f, 0.f, 0.f, 1.f, 0.f,

        0.f, 0.f, -1.f, 0.f, 1.f,
        0.f, 1.f, -1.f, 0.f, 0.f,
        1.f, 1.f, -1.f, 1.f, 0.f,
        1.f, 0.f, -1.f, 1.f, 1.f,
    };

    unsigned int indices[] = {
        //front
        0, 1, 2,
        2, 3, 0,

        //top
        1, 5, 6,
        6, 2, 1,

        //right
        3, 2, 6,
        6, 7, 3,

        //bottom
        0, 4, 7,
        7, 3, 0,

        //left
        0, 1, 5,
        5, 4, 0,

        //back
        4, 5, 6,
        6, 7, 4
    };

    unsigned int VAO3, VBO3, EBO3;

    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);
    glGenBuffers(1, &EBO3);

    glBindVertexArray(VAO3);

    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE(5, float), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, STRIDE(5, float), BUFFER_OFFSET(3*sizeof(float)));
    
    glBindVertexArray(0);//*/
    
    GenTexture2D image3("/home/arno/opengl/src/textures/halo-infinite.jpg", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST);
    
    Shader shaderCube("/home/arno/opengl/src/shader/vertexCube.glsl", "/home/arno/opengl/src/shader/fragTextureShader.glsl");

    /*shaderProgramTexture.use();
    shaderProgramTexture.setFloat("t", 2.f);
    glUniform1i(glGetUniformLocation(shaderProgramTexture.ID, "T"), 0);
    glUniform1i(glGetUniformLocation(shaderProgramTexture.ID, "text2"), 1);
    unsigned int mLoc = glGetUniformLocation(shaderProgramTexture.ID, "m");//*/


    glClearColor(0.f, 0.3f, 0.3f, 1.f);
    glEnable(GL_DEPTH_TEST);
    int width;
    int height;
    glfwGetWindowSize(window, &width, &height);

    // * Model matrix : translations, scaling, rotations
    mat4 rot1;
    mat4 rot2;
    mat4 trans;
    trans = translate(trans, vec3(-0.5f, 0.f, 0.f));

    // * View matrix : doesn't really moves the camera, it moves the entire scene
    mat4 view;
    view = translate(view, vec3(0.f, 0.f, -4.f));


    // * Projections matrix : frustums definitions
    mat4 orth = ortho(0.f, 600.f, 0.f, 800.f, 0.1f, 100.f);
    mat4 persp = perspective(radians(50.f), (float)width / (float)height, 0.1f, 100.f);

    shaderCube.use();

    glUniform1i(glGetUniformLocation(shaderCube.ID, "T"), 0);

    unsigned int modelLoc = glGetUniformLocation(shaderCube.ID, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderCube.ID, "view");
    unsigned int projLoc = glGetUniformLocation(shaderCube.ID, "proj");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(rot1));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(persp));
    



    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        process_input(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
        glfwGetWindowSize(window, &width, &height);
        shaderCube.use();

        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, image3.textID);
        rot1 = rotate(rot1, (float)(cos(glfwGetTime()) / 10.f), vec3(1.f, 0.f, 0.f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(trans * rot1));
        /*glUniform1f(glGetUniformLocation(shaderProgramTexture.ID, "yAdjust"), (height > width) ? (float)width / (float)height : 1.f);
        glUniform1f(glGetUniformLocation(shaderProgramTexture.ID, "xAdjust"), (width > height) ? (float)height / (float)width : 1.f);
        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, sizeof(circleIndices) / sizeof(*circleIndices), GL_UNSIGNED_INT, 0);//*/
        glBindVertexArray(VAO3);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(*indices), GL_UNSIGNED_INT, 0);

        rot2 = rotate(rot2, (float)(cos(glfwGetTime()) / 100.f), vec3(1.f, 1.f, 0.f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(trans * trans * trans *  trans));
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(*indices), GL_UNSIGNED_INT, 0);

        
        glfwSwapBuffers(window);
    }//*/


    glfwTerminate();
    return 0;
}

