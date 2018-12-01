#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "window/window.h"
#include "camera/camera.h"
#include "drawing/surface.hpp"
#include "drawing/cube.hpp"
#include "shader/shader.h"
#include "textures/procedural/noise.hpp"

#define SCR_HEIGHT 900
#define SCR_WIDTH 900

using namespace glm;

void MouseCallback(GLFWwindow *window, double xpos, double ypos);

Camera cam;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "water", NULL, NULL);
    if (window == NULL)
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

    mat4 persp = perspective(radians(80.0f), (float)(SCR_WIDTH) / (float)(SCR_HEIGHT), 0.1f, 100.0f);
    Surface surface(300, &cam, &persp);
    float D = 0.2f;
    float H = .2f;

    Shader shaderWater("/home/arno/Proj/opengl/src/shader/vertex/water.vs.glsl", "/home/arno/Proj/opengl/src/shader/fragment/water.fs.glsl");
    shaderWater.use();
    shaderWater.setFloat("D", D);
    shaderWater.setFloat("H", H);
    shaderWater.setVec3("diretionalLight.ambient", vec3(.1f, .1f, .1f));
    shaderWater.setVec3("diretionalLight.diffuse", vec3(1.0f, 1.0f, 1.0f));
    shaderWater.setVec3("diretionalLight.direction", vec3(-1.0f, -1.1f, 0.0f));
    shaderWater.setVec3("material.ambient", vec3(0.686f, 0.933f, 0.933f));
    shaderWater.setVec3("material.diffuse", vec3(0.686f, 0.933f, 0.933f));
    shaderWater.setVec3("material.specular", vec3(0.686f, 0.933f, 0.933f));
    shaderWater.setFloat("material.shininess", 128.0f);

    mat4 model;
    model = scale(model, vec3(5, 1, 5));
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // * UBO
    unsigned int UboWaveBlock;
    glGenBuffers(1, &UboWaveBlock);
    glBindBuffer(GL_UNIFORM_BUFFER, UboWaveBlock);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float), sizeof(float), &D);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(float), sizeof(float), &H);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, UboWaveBlock);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    unsigned int UniWaveLocation = glGetUniformBlockIndex(shaderWater.ID, "Wave");
    glUniformBlockBinding(shaderWater.ID, UniWaveLocation, 0);

    unsigned int UboMatricesBlock;
    glGenBuffers(1, &UboMatricesBlock);
    glBindBuffer(GL_UNIFORM_BUFFER, UboMatricesBlock);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(mat4), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), value_ptr(persp));
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, UboMatricesBlock);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    unsigned int UniMatricesLoc = glGetUniformBlockIndex(shaderWater.ID, "Matrices");
    glUniformBlockBinding(shaderWater.ID, UniMatricesLoc, 1);

    unsigned int UboCameraBlock;
    glGenBuffers(1, &UboCameraBlock);
    glBindBuffer(GL_UNIFORM_BUFFER, UboCameraBlock);
    glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, UboCameraBlock);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    unsigned int UniCameraLocation = glGetUniformBlockIndex(shaderWater.ID, "Camera");
    glUniformBlockBinding(shaderWater.ID, UniCameraLocation, 2);

    Cube *cube = new Cube();
    std::cout << cube->GetVerticesCount() << std::endl;

    GLsizei noiseDimension = 16;
    Noise noise(noiseDimension, noiseDimension);
    shaderWater.use();

    // SECTION Noise
    float amplitude = 1.f;
    float frequency = .2f;
    for (size_t i = 0; i < 8; i++)
    {
        std::string s = "noise[" + std::to_string(i) + "].";
        shaderWater.setInt(s + "texture", 0);
        shaderWater.setInt(s + "width", noiseDimension);
        shaderWater.setInt(s + "height", noiseDimension);
        shaderWater.setFloat(s + "amplitude", amplitude);
        shaderWater.setFloat(s + "frequency", frequency);
        amplitude *= .35f;
        frequency *= 1.8f;
    }
    // !SECTION
    glActiveTexture(GL_TEXTURE0);
    noise.BindNoise();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera_input(window, &cam);

        glBindBuffer(GL_UNIFORM_BUFFER, UboWaveBlock);
        float t = glfwGetTime() * .5f;
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &t);

        glBindBuffer(GL_UNIFORM_BUFFER, UboMatricesBlock);
        mat4 view = cam.GetViewMatrix();
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), value_ptr(view));

        glBindBuffer(GL_UNIFORM_BUFFER, UboCameraBlock);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, 4 * sizeof(float), value_ptr(cam.position));

        shaderWater.use();
        shaderWater.setVec3("camPos", cam.position);
        surface.Draw(&model, shaderWater);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void MouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    (void)window;
    static float xOld = xpos;
    static float yOld = ypos;
    cam.ProcessMouseMouvement(xpos - xOld, yOld - ypos);
    xOld = xpos;
    yOld = ypos;
}