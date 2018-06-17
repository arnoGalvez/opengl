#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
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
    float vertices[] = {
        //positions         //colors                   
        0.5f, 0.5f, 0.0f,   1.0f, .0f, 1.0f, 1.0f,   1.f, 1.f, // top right
        0.5, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,  1.f, 0.f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f, 1.0f,    0.f, 0.f,// bottom left
        -0.5f, 0.5f, 0.0f,  .0f, .0f, .0f, 1.0f,    0.f, 1.f// top left
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

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT), BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
    //5: we tell the vertex attrib array to activate a third attrib
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIDE(9, GL_FLOAT), BUFFER_OFFSET(7 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);    

    GenTexture2D image1("/home/arno/opengl/src/textures/halo-infinite.jpg", GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
    GenTexture2D image2("/home/arno/opengl/src/textures/halo-generic-logo.png", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
    
    glBindVertexArray(0);

    shaderProgramTexture.use();
    shaderProgramTexture.setFloat("t", 2.f);
    glUniform1i(glGetUniformLocation(shaderProgramTexture.ID, "text1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgramTexture.ID, "text2"), 1);


    glClearColor(0.f, 3.f, 0.f, 1.f);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        process_input(window);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgramTexture.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, image1.textID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, image2.textID);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

