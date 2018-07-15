#include "window.h"

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void camera_input(GLFWwindow *window, Camera *cam)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glad_glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glad_glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    float deltaTime = GetDeltaTime();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        (*cam).ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        (*cam).ProcessKeyboard(BACKWARD, deltaTime);
    }    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        (*cam).ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        (*cam).ProcessKeyboard(RIGHT, deltaTime);
    } 
    
}//*/

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    (void) window;
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window, float speed, vec3* translation)
{
    float deltaTime = GetDeltaTime();
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        translation->x += speed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        translation->x -= speed * deltaTime;
    }
}

float GetDeltaTime()
{
    static float lastFrame;
    static float currentFrame;
    static float deltaTime;
    lastFrame = currentFrame;
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    return deltaTime;
}


