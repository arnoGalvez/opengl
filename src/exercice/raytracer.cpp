#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdlib.h>

#include "window/window.h"
#include "camera/camera.h"
#include "shader/shader.h"
#include "textures/import.h"
#include "raytracer/ray.hpp"

#define SCR_HEIGHT 900
#define SCR_WIDTH 900

using namespace glm;

void MouseCallback(GLFWwindow* window, double xpos, double ypos);

Camera cam;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Spheres drawn using basic raytracing", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "ERROR::GLFW::NULL WINDOW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, MouseCallback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

    // * Data
    // * ----
    mat4 persp = perspective(radians(80.0f), (float)(SCR_WIDTH) / (float)(SCR_HEIGHT), 0.1f, 100.0f);
    mat4 perspInv = inverse(persp);
    Shader shaderRay("/home/arno/Proj/opengl/src/shader/vertex/raytracer.glsl","/home/arno/Proj/opengl/src/shader/fragment/raytracer.glsl");
    shaderRay.use();
    shaderRay.setMat4("perspInv", perspInv);
    shaderRay.setInt("spheresCount", 3);
    shaderRay.setFloat("spheres[0].radius", .5f);
    shaderRay.setFloat("spheres[1].radius", 100.0f);
    shaderRay.setFloat("spheres[2].radius", .5f);
    shaderRay.setVec3("spheres[0].material.color", vec3(0.2f, 0.3f, 0.8f));
    shaderRay.setInt("spheres[0].material.type", 0);
    shaderRay.setVec3("spheres[1].material.color", vec3(0.3f, 0.2f, 0.8f));
    shaderRay.setInt("spheres[1].material.type", 0);
    shaderRay.setVec3("spheres[2].material.color", vec3(0.8f, 0.6f, 0.2f));
    shaderRay.setInt("spheres[2].material.type", 1);
    shaderRay.setFloat("spheres[2].material.fuzz", 1.f);
    
    

    // * Quad
    // * ----
    float corners[] = {
        -1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, -1.0f
    };
    unsigned int indices[] = {
        2,1,0,
        2,0,3
    };
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(corners), corners, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glBindVertexArray(0);

    // * Noise
    // * -----
    // GenTexture2D Noise("/home/arno/Proj/opengl/src/textures/noise.png", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
    // shaderRay.use();
    // shaderRay.setInt("Noise", 0);//*/
    
    
   
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera_input(window, &cam);

        shaderRay.use();
        mat4 view;
        view = cam.GetViewMatrix();
        shaderRay.setVec4("spheres[0].center", view * vec4(2.0f, .5f, 0.0f, 1.0f));
        shaderRay.setVec4("spheres[1].center", view * vec4(2.0f, -100.0f, 0.0f, 1.0f));
        shaderRay.setVec4("spheres[2].center", view * vec4(2.0f, .5f, 1.0f, 1.0f));
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, Noise.textID);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        glfwSwapBuffers(window);
        
    }
    
    glfwTerminate();
    return 0;
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    (void)window;
    static float xOld = xpos;
    static float yOld = ypos;
    cam.ProcessMouseMouvement(xpos - xOld, yOld - ypos);
    xOld = xpos;
    yOld = ypos;
}
