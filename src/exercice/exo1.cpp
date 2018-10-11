#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "shader/shader.h"
#include "window/window.h"
#include "drawing/drawing.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Shooting Stars", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // creating the shader program
    Shader shaderProgramColor("/home/arno/opengl/src/shader/colorShader.vs", "/home/arno/opengl/src/shader/colorShader.fs");

    // creating a VAO for the triangle
    //--------------------------------
    float vertices_1[] = {
        -0.5f, 0.8f, 0.f,
        0.5f, 0.8f, 0.f,
        0.f, 0.f, 0.f,
        0.5, -0.8f, 0.f,
        -0.5, -0.8f, 0.f
    };

    unsigned int indices_1[] = {
        0, 1, 2,
    };

    unsigned int VBO, VAO, EBO; // vertex buffer object, allow to send a large number of vertices to GPU at once
                           // vertex array object, to store vertex attributes calls == input for the pipeline
                           // element buffer object, inidices corresponding to vertices
    glGenVertexArrays(1, &VAO);// number of vertex array objects to be created -- array of identifiers
    glGenBuffers(1, &VBO);// number of vertex buffer objects to be created -- array of identifiers
    glad_glGenBuffers(1, &EBO);
    
    glad_glBindVertexArray(VAO);

    // 0. copy vertices into the buffer VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);//4th param : the data will most likely not change at all or very rarely

    // 1. copy index array into the buffer EBO
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_1), indices_1, GL_STATIC_DRAW);

    // 2. How the vertex data should be treated / vertex attribute
    glad_glEnableVertexAttribArray(0);// enable the attribute at location 0
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);

    // the call to glVertexAttribPointer bound VBO to be the vertex buffer object asociated to those certex attributes
    // so we can unbind VBO
    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);


    glad_glBindVertexArray(0);
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    unsigned int VBO_2, VAO_2, EBO_2;
    Triangle2(&VAO_2, &VBO_2, &EBO_2, vertices_1, indices_1);









    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };

    unsigned int VAO_C, VBO_C;
    glad_glGenVertexArrays(1, &VAO_C);
    glad_glGenBuffers(1, &VBO_C);

    glad_glBindVertexArray(VAO_C);

    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO_C);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glad_glEnableVertexAttribArray(0);
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(1);
    glad_glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GL_FLOAT)));

    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);




    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        //process_input(window);

        // render
        // ------
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // drawing !!!!! <3
        //-----------------
        //glUseProgram(shaderProgramYellow);
//
//        //float timeValue = glfwGetTime();
//        //float yellowValue = (sin(timeValue) / 2.0f) + 0.5f;
//        //int vertexColorLocation = glGetUniformLocation(shaderProgramYellow, "ourColor");
//        //glUniform4f(vertexColorLocation, yellowValue, yellowValue, 0.0f, 1.0f);
//        //
//        //glad_glBindVertexArray(VAO);
//        //glad_glDrawArrays(GL_TRIANGLES, 3, 3);
//        //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)0);
//
//        //glad_glBindVertexArray(VAO_2);
//        //glad_glDrawArrays(GL_TRIANGLES, 0, 3);
//        //
        //glad_glBindVertexArray(0);

        shaderProgramColor.use();
        glad_glBindVertexArray(VAO_C);
        glad_glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}