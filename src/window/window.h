#ifndef __WINDOW_H
#define __WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "camera/camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void camera_input(GLFWwindow *window, Camera *cam);
void process_input(GLFWwindow *window, float speed, vec3* translation);
float GetDeltaTime();

#endif