#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <cmath>

#include "shader.h"
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

    

    float vertices[] = {
        //positions         //colors                   //textures coords
        0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, // top right
        0.5, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f  // top left
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Texture 2D
    //-----------

    // generating Texture
    unsigned int texture[2];
    glGenTextures(2, texture);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    // specify sampling on both axes of our texture 2D
    float color[] = {1.0f, 0.f, 0.f, 1.f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // specify how to minify with the mipmaps, how to magnify : texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    // loading an image
    int width, height, nrChannels;
    // to flip the himage vertically
    stbi_set_flip_vertically_on_load(true);  
    unsigned char * image_data[2];
    char doom[] = "container.jpg";
    image_data[0] = stbi_load(doom, &width, &height, &nrChannels, 0);
    if (image_data[0])
    {
        // 2nd param : Mipmap level / 3rd param : nb of color components / 7th param : format of the pixel data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data[0]);
        //setting the mipmap
        glGenerateMipmap(GL_TEXTURE_2D);
    } 
    else 
    {
        std::cout << "Failed to load " << doom << std::endl;
    }

    // good practice ! :
    stbi_image_free(image_data[0]);
    
    // 2nd image
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    char philip[] = "awesomeface.png";
    image_data[1] = stbi_load(philip, &width, &height, &nrChannels, 0);
    if (image_data[1])
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data[1]);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "falied to load " << philip << std::endl;
    }

    stbi_image_free(image_data[1]);

    glBindVertexArray(0);

    Shader shaderProgramTexture("/home/arno/opengl/src/shader/textureShader.vs", "/home/arno/opengl/src/shader/textureShader.fs");

    // setting texture units
    shaderProgramTexture.use(); // dont't forget it !!! Otherwise the next lines won't work !
    glUniform1i(glGetUniformLocation(shaderProgramTexture.ID, "texture1"), 0);
    shaderProgramTexture.setInt("texture2", 1);

    float x = 0.0f;
    float y = 0.0f;
    char xDir = LEFT;
    char yDir = UP;
    float time1 = 0.0f;
    float time2 = 0.0f;
    float deltaTime;
    float speedMultiplier = 1.f;

    while(!glfwWindowShouldClose(window))
    {
        process_input(window);
        time1 = time2;
        time2 = glfwGetTime();
        deltaTime = time2 - time1;
        XMove(&x, &xDir, speedMultiplier, deltaTime);
        YMove(&y, &yDir, speedMultiplier / 2, deltaTime);
        
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        shaderProgramTexture.use();
        shaderProgramTexture.setFloat("xSpeed", x);
        shaderProgramTexture.setFloat("ySpeed", y);
        shaderProgramTexture.setFloat("r", cosf(time2));
        shaderProgramTexture.setFloat("g", sinf(time2));
        shaderProgramTexture.setFloat("b", tanf(time2));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}