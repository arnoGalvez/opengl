#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader/shader.h"
#include "textures/import.h"
#include "window/window.h"
#include "camera/camera.h"
#include "assimp/mesh.h"
#include "LoadedObjects/model.h"
#include "textures/cubemaps/cubemap.h"

using namespace glm;

#define BUFF_OFFSET(offset) ((void *)(offset))
#define STRIDE(n, type) (n * (sizeof(type)))

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

Camera cam;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Halo Infinite", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ! Mouse Input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    float cube[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    unsigned int VAO_cube, VBO1;

    glGenVertexArrays(1, &VAO_cube);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(VAO_cube);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE(8, float), BUFF_OFFSET(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE(8, float), BUFF_OFFSET(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIDE(8, float), BUFF_OFFSET(6 * sizeof(float)));

    glBindVertexArray(0); //*/

    //GenTexture2D image3("/home/arno/opengl/src/textures/halo-infinite.jpg", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, true);

    Shader shaderCube("/home/arno/opengl/src/shader/vertex/vertexCube.glsl", "/home/arno/opengl/src/shader/fragment/fragCube.glsl");
    Shader shaderLamp("/home/arno/opengl/src/shader/vertex/vertexLight.glsl", "/home/arno/opengl/src/shader/fragment/fragLight.glsl");

    // ! setting some parameters
    glClearColor(0.f, 1.f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST); // ! don't forget the z-buffer
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xff);                  // * value ANDed with the entire stencil buffer
    glStencilFunc(GL_ALWAYS, 0xff, 0xff); // * describes the stencil test
    glStencilOp(GL_KEEP, GL_ZERO, GL_REPLACE);
    // !

    // * Model matrix : translations, scaling, rotations
    mat4 trans;
    trans = translate(trans, vec3(-0.5f, 0.f, 0.f));

    // * View matrix : doesn't really moves the camera, it moves the entire scene
    mat4 view;
    view = translate(view, vec3(0.f, 0.f, -4.f));

    // * Projections matrix : frustums definitions
    mat4 orth = ortho(0.f, 600.f, 0.f, 800.f, 0.1f, 100.f);
    mat4 persp = perspective(radians(80.f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f);

    vec3 cameraPos = vec3(0.f, 0.f, 5.f);
    vec3 cameraTarget = vec3(0.5f, 0.5f, 0.f);
    vec3 worldUp = vec3(0.f, 1.f, 0.f);

    shaderCube.use();
    shaderCube.setMat4("proj", persp);

    shaderCube.setVec3("material.ambient", vec3(0.19225f, 0.19225f, 0.19225));
    //shaderCube.setVec3("material.diffuse", vec3(0.50754f, 0.50754f, 0.50754f));
    shaderCube.setInt("material.diffuse", 0);
    //shaderCube.setVec3("material.specular", vec3(0.508273f, 0.508273f, 0.508273f));
    shaderCube.setInt("material.specular", 1);
    shaderCube.setInt("material.emission", 2);
    shaderCube.setFloat("material.shininess", 0.6 * 128.f);

    shaderCube.setVec3("directional.direction", vec3(-1, -1, 0));
    shaderCube.setVec3("directional.ambient", vec3(0.1f, 0.1f, 0.1f));
    shaderCube.setVec3("directional.diffuse", vec3(1, 1, 1));
    shaderCube.setVec3("directional.specular", vec3(1, 1, 1));

    shaderCube.setFloat("spotAngleInner", glm::cos(radians(12.5f)));

    shaderLamp.use();
    vec3 lightTranslation = vec3(10, 5, 0);
    shaderLamp.setMat4("proj", persp);
    shaderLamp.setVec3("lightColor", vec3(1.0f, 1.0f, 1.0f));

    // Textures
    //---------
    GenTexture2D container("/home/arno/opengl/src/textures/container2.png", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_NEAREST_MIPMAP_LINEAR);
    GenTexture2D containerSpecular("/home/arno/opengl/src/textures/container2_specular.png", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_NEAREST_MIPMAP_LINEAR);
    GenTexture2D emissionMap("/home/arno/opengl/src/textures/matrix.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_NEAREST_MIPMAP_LINEAR, true);

    // Moar cubes !
    //-------------

    vec3 cubePositions[] = {
        vec3(0, 0, 0),
        vec3(0, 1, 0),
        vec3(0, 0, 1),
        vec3(0, -1, 0),
        vec3(0, 0, -1),
    };

    // Moar lights !
    //--------------
    vec3 pointLightPositions[] = {
        vec3(0.7f, 0.2f, 2.0f),
        vec3(2.3f, -3.3f, -4.0f),
        vec3(-4.0f, 2.0f, -4.0f),
        vec3(0.0f, 3.0f, -3.0f)};

    shaderCube.use();
    for (int i = 0; i < 4; i++)
    {
        std::string p1 = "pointLights[";
        std::string s = std::to_string(i);
        std::string p2 = "].";
        shaderCube.setVec3(p1 + s + p2 + "position", 4.0f * pointLightPositions[i]);
        shaderCube.setVec3(p1 + s + p2 + "ambient", vec3(0.1f, 0.1f, 0.1f));
        shaderCube.setVec3(p1 + s + p2 + "diffuse", vec3(1, 0, 0));
        shaderCube.setVec3(p1 + s + p2 + "specular", vec3(1, 1, 1));
        shaderCube.setFloat(p1 + s + p2 + "kC", 1);
        shaderCube.setFloat(p1 + s + p2 + "kL", 0.00f);
        shaderCube.setFloat(p1 + s + p2 + "kQ", 0.000f);
    }
    //*/
    // ! Nanosuit
    //!----------
    Shader shaderNanosuit("/home/arno/opengl/src/shader/vertex/vertexCube.glsl", "/home/arno/opengl/src/shader/fragment/nanosuit.glsl");
    shaderNanosuit.use();
    shaderNanosuit.setMat4("proj", persp);
    char *nanopath = "/home/arno/opengl/src/LoadedObjects/nanosuit/nanosuit.obj";
    Model nanosuit = Model(nanopath);
    //*/

    float square[] = {-1, 0, -1, 0, 1, 0, 0, 0, -1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, -1, 0, 1, 0, 1, 0};

    unsigned int squareIndices[] = {
        0, 1, 2,
        2, 3, 0};

    // * steel slab -------
    //*--------------------
    unsigned int VAO_square, VBO_square, EBO_square;
    glGenVertexArrays(1, &VAO_square);
    glBindVertexArray(VAO_square);
    glGenBuffers(1, &VBO_square);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_square);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
    glGenBuffers(1, &EBO_square);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_square);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFF_OFFSET(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFF_OFFSET(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFF_OFFSET(6 * sizeof(float)));

    glBindVertexArray(0);

    GenTexture2D steel("/home/arno/opengl/src/textures/steel.jpg", GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
    Shader slabShader("/home/arno/opengl/src/shader/vertex/vertexCube.glsl", "/home/arno/opengl/src/shader/fragment/slab.glsl");
    slabShader.use();
    slabShader.setMat4("proj", persp);
    slabShader.setInt("image", 0);

    // * halo2 
    //*-------
    GenTexture2D halo2("/home/arno/opengl/src/textures/Halo_2_Logo.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);

    // * window 
    //*--------
    GenTexture2D littleWindow("/home/arno/opengl/src/textures/blending_transparent_window.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
    vector<vec3> windowsPositions;
    int windowY = 5;
    windowsPositions.push_back(vec3(10, windowY, 10));
    windowsPositions.push_back(vec3(8, windowY, 12));
    windowsPositions.push_back(vec3(12, windowY, 14));

    // * Framebuffer 
    //*-------------
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    int width;
    int height;
    glfwGetWindowSize(window, &width, &height);
    unsigned int frameBufTexture;
    glGenTextures(1, &frameBufTexture);
    glBindTexture(GL_TEXTURE_2D, frameBufTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    // ! play with glViewPort to render a specific part of the scene to the texture, or to render the screen to a smaller texture!!
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufTexture, 0);
    // ! possible to add a texture combining both depth & stencil buffers ! But it's less efficient than a renderbuffer, because of the conversion when writing data.
    /*unsigned int frameBufferDepthSten;
    glGenTextures(1, &frameBufferDepthSten);
    glBindTexture(GL_TEXTURE_2D, frameBufferDepthSten);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, frameBufferDepthSten, 0);*/
    // ! more efficient depth & stencil buffers, with data written in OpenGl internal format (thus not converted)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    // ! general rule: u need to sample from a buffer ? use a texture. Else use a renderbuffer.

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not completed" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float screen[] = {
        -1.f, -1.f, 0.f, 0.f,
        -1.f, 1.f, 0.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, -1.f, 1.f, 0.f};
    unsigned int screenIndices[] = {
        0, 2, 1,
        0, 3, 2};
    unsigned int VAO_screen, VBO_screen, EBO_screen;
    glGenVertexArrays(1, &VAO_screen);
    glGenBuffers(1, &VBO_screen);
    glGenBuffers(1, &EBO_screen);
    glBindVertexArray(VAO_screen);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_screen);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_square);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screen), screen, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(screenIndices), screenIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), BUFF_OFFSET(2 * sizeof(float)));
    glBindVertexArray(0);

    Shader shaderFrameBuffer("/home/arno/opengl/src/shader/vertex/frameBuffer.glsl", "/home/arno/opengl/src/shader/fragment/frameBuffer.glsl");

    // * outline 
    //*---------
    Shader outline = Shader("/home/arno/opengl/src/shader/vertex/outline.glsl", "/home/arno/opengl/src/shader/fragment/outline.glsl");
    outline.use();
    outline.setMat4("proj", persp);

    // * cubemap 
    //*---------
    float skyboxVertices[] = {-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};
    vector<string> cubeMapTextures;
    cubeMapTextures.push_back("/home/arno/opengl/src/textures/cubemaps/hell_ft.tga");
    cubeMapTextures.push_back("/home/arno/opengl/src/textures/cubemaps/hell_bk.tga");
    cubeMapTextures.push_back("/home/arno/opengl/src/textures/cubemaps/hell_up.tga");
    cubeMapTextures.push_back("/home/arno/opengl/src/textures/cubemaps/hell_dn.tga");
    cubeMapTextures.push_back("/home/arno/opengl/src/textures/cubemaps/hell_rt.tga");
    cubeMapTextures.push_back("/home/arno/opengl/src/textures/cubemaps/hell_lf.tga");
    Cubemap cubemap(cubeMapTextures, 8 * 36, skyboxVertices, &persp);
    shaderLamp.use();
    shaderLamp.setInt("skybox", 0);

    //*Messin' around
    //*--------------

    glEnable(GL_PROGRAM_POINT_SIZE);





































   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        camera_input(window, &cam);
        process_input(window, 50000.f, &lightTranslation);

        glfwGetWindowSize(window, &width, &height);
        mat4 camMat = cam.GetViewMatrix();
        vec3 camPos = cam.position;

        // ! Framebuffer
        //!-------------

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClearColor(0.f, 1.f, 0.f, 1.0f),

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glViewport(0.f, 0.f, SCR_WIDTH, SCR_HEIGHT);


        // Cube
        //-----
        /*shaderCube.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, container.textID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, containerSpecular.textID);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, emissionMap.textID);
        shaderCube.setMat4("view", camMat);
        shaderCube.setVec3("point.position", lightTranslation);
        shaderCube.setVec3("camPos", cam.position);
        shaderCube.setVec3("camForward", cam.front);

        //glStencilFunc(GL_ALWAYS, 0xff, 0xff);
        //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        /*for(int i = 0; i < sizeof(cubePositions) / sizeof(*cubePositions); i++)
        {
            mat4 scale3;
            scale3 = scale(scale3, vec3(4, 4, 4));
            mat4 pos;
            pos = translate(pos, 10.0f * cubePositions[i]);
            mat4 rot;
            rot = rotate(rot, (float)radians(glfwGetTime() / (i + 1)) * 20.f, vec3(1, 1, i));
            shaderCube.setMat4("model", pos * rot * scale3);
            glBindVertexArray(VAO_cube);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));
        }//*/

        /*glStencilFunc(GL_EQUAL, 0, 0xff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        temp.use();
        temp.setMat4("view", camMat);
        temp.setMat4("proj", persp);
        for(int i = 0; i < sizeof(cubePositions) / sizeof(*cubePositions); i++)
        {
            mat4 scale3;
            scale3 = scale(scale3, vec3(4.2f, 4.2f, 4.2f));
            mat4 pos;
            pos = translate(pos, 10.0f * cubePositions[i]);
            mat4 rot;
            rot = rotate(rot, (float)radians(glfwGetTime() / (i + 1)) * 20.f, vec3(1, 1, i));
            temp.setMat4("model", pos * rot * scale3);
            glBindVertexArray(VAO_cube);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));
        }//*/

        // ! Lamp
        //*------
        shaderLamp.use();
        shaderLamp.setMat4("view", camMat);
        shaderLamp.setVec3("camPos", cam.position);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.Id);

        for (int i = 0; i < 4; i++)
        {
            mat4 trans;
            trans = translate(trans, pointLightPositions[i]);
            shaderLamp.setMat4("model", trans);
            glBindVertexArray(VAO_cube);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));
        }//*/

        // ! slab
        //*------
        /*slabShader.use();
        slabShader.setMat4("view", camMat);
        mat4 slabScale;
        slabScale = scale(slabScale, vec3(32, 32, 32));
        slabShader.setMat4("model", slabScale);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, steel.textID);
        glBindVertexArray(VAO_square);
        glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(*squareIndices), GL_UNSIGNED_INT, 0);//*/

        // ! halo2
        //*-------
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
        slabShader.use();
        mat4 grassRotX;
        mat4 grassTrans;
        grassTrans = translate(grassTrans, vec3(10, 7, 11));
        grassRotX = rotate(grassRotX, radians(-90.0f), vec3(1, 0, 0));
        slabShader.setMat4("model", grassTrans * grassRotX);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, halo2.textID);
        glBindVertexArray(VAO_square);
        glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(*squareIndices), GL_UNSIGNED_INT, 0);
        glDisable(GL_BLEND);
        //* */

        // ! nanaosuit
        //*-----------
        /*glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);//*/
        glFrontFace(GL_CCW); // counter clock wise*/
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 1, 0xff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        shaderNanosuit.use();
        shaderNanosuit.setMat4("view", camMat);
        mat4 scal;
        scal = scale(scal, vec3(.6f, .6f, .6f));
        mat4 tmp;
        tmp = translate(tmp, vec3(0, 0, 30));
        scal = tmp * scal;
        shaderNanosuit.setMat4("model", scal);
        shaderNanosuit.setVec3("camPos", cam.position);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.Id);
        shaderNanosuit.setInt("skybox", 3);
        nanosuit.Draw(shaderNanosuit);
        glDisable(GL_CULL_FACE);

        glStencilFunc(GL_EQUAL, 0, 0xff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        outline.use();
        outline.setMat4("view", camMat);
        outline.setMat4("model", scal);
        nanosuit.Draw(outline);
        glDisable(GL_STENCIL_TEST);

        // ! windows
        //*---------
        /*glEnable(GL_BLEND);
        glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

        slabShader.use();
        mat4 winScale;
        winScale = scale(winScale, vec3(5,5,5));
        mat4 winRot;
        winRot = rotate(winRot, radians(-90.0f), vec3(1,0,0));
        for (size_t i = 0; i < windowsPositions.size(); i++)
        {
            mat4 winTrans;
            winTrans = translate(winTrans, windowsPositions[i]);
            slabShader.setMat4("model", winTrans * winScale * winRot);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, littleWindow.textID);
            glBindVertexArray(VAO_square);
            glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(*squareIndices), GL_UNSIGNED_INT, 0);
        }
        glDisable(GL_BLEND);//* */

        //*Cubemap
        //*-------
        cubemap.Draw(&camMat);

        //*Framebuffer
        //*-----------
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(1.0f, 0.f, 1.f, 1.0f),
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        shaderFrameBuffer.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, frameBufTexture);
        glBindVertexArray(VAO_screen);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        /*glViewport(0, 0, SCR_WIDTH / 2, SCR_HEIGHT / 2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);*/
        glEnable(GL_DEPTH_TEST);



        glfwSwapBuffers(window);
    } //*/

    glDeleteFramebuffers(1, &fbo);
    glDeleteVertexArrays(1, &VAO_cube);
    glDeleteVertexArrays(1, &VAO_square);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &VBO_square);
    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    (void)window;
    static float x = xpos;
    static float y = ypos;

    cam.ProcessMouseMouvement(xpos - x, y - ypos);
    x = xpos;
    y = ypos;
}