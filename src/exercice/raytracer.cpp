#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "window/window.h"
#include "camera/camera.h"
#include "shader/shader.h"

#define SCR_HEIGHT 600
#define SCR_WIDTH 800

using namespace glm;

void MouseCallback(GLFWwindow* window, double xpos, double ypos);

Camera cam;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "water", NULL, NULL);
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
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

    // * Data
    // * ----
    mat4 persp = perspective(radians(80.0f), (float)(SCR_WIDTH) / (float)(SCR_HEIGHT), 0.1f, 100.0f);
    
   
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera_input(window, &cam);


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
