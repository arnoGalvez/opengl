#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "Textures/import.h"
#include "window/window.h"

#define BUFFER_OFFSET(offset) ((void*) (offset))


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

    float vertices[] = {
        //positions         //colors                   
        0.5f, 0.5f, 0.0f,   1.0f, .0f, 1.0f, 1.0f,    // top right
        0.5, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,    // bottom right
        -0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f, 1.0f,    // bottom left
        -0.5f, 0.5f, 0.0f,  .0f, .0f, .0f, 1.0f,    // top left
    };

    unsigned int indices[] = {
        0,1,3,1,2,3
    };

    // 1 : a bunch of coordinates in a texture, ie parameters for
    // the function mapping a point to a color
    float textCoords[] = {
        1.f, 1.f,
        1.f, 0.f,
        0.f, 0.f,
        0.f, 1.f
    };

    // 2 : how the function should perform outside it's range ?
    // what type of texture is being affected, 
    // along which axis
    // what behaviour should be assigned
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    //GL_CLAMP_TO_BORDER is a bit special : we have to specify a color !
    float aColor[] = {1.f, 0.f, 0.f, 1.f};
    glTexParameterfv(GL_TEXTURE_2D, GL_CLAMP_TO_BORDER, aColor);

    // 2: how the function actually chooses which texel to return given a point (x,y) ?
    //we look were falls that coordinate on the texture, and then we have to decide what to do:
    //first, if the image we want to paint is the size of the texture, no problem, avery coordinate unimbanguously
    //corresponds to a texel.
    //else, we can choose to interpolates between the neighbooring texels, or output the nearest.

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 3: 2 showed one thing : mapping (x,y) to a texel can be tricky.
    // Imagine an object very small tries to sample from an high res texture, retrieveing the right pixel could be costly
    // because right texel x-axis = point.x * num of texels on x-axis, and that last number could be big !
    // so: mipmaps ! a texture is regrouped with it's sisters, each one being half the size of the previous one
    // well of course you will probably paint an object not the size of any of those txtures, so we have to choose
    // a way to chose which one(s) to take : the nearest one, or a linear interpolation of the the texels of two mipmaps.
    // we choose the texels as in 2.

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    // 4: generating the texture. Please note UNSIGNED
    unsigned int myTexture;
    glGenTextures(1, &myTexture);
    glBindTexture(GL_TEXTURE_2D, myTexture);

    GenTexture2D image("src/Textures/halo-generic-logo.png");
    





    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
    
    glBindVertexArray(0);

    Shader shaderProgramTexture("/home/arno/opengl/src/shader/vertexColorShader.glsl", "/home/arno/opengl/src/shader/fragColorShader.glsl");
    glClearColor(0.f, 3.f, 0.f, 1.f);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        process_input(window);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgramTexture.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

