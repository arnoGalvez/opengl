#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "textures/import.h"
#include "window/window.h"
#include "camera/camera.h"
//#include "assimp/mesh.h"
//#include "LoadedObjects/model.h"

using namespace glm;

#define BUFF_OFFSET(offset) ((void*) (offset))
#define STRIDE(n, type) (n * (sizeof(type)))

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

Camera cam;

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

    // ! Mouse Input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);


    float cube[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};



    unsigned int VAO1, VBO1;

    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE(8, float), BUFF_OFFSET(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE(8, float), BUFF_OFFSET( 3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIDE(8, float), BUFF_OFFSET(6 * sizeof(float)));
    
    glBindVertexArray(0);//*/
    
    //GenTexture2D image3("/home/arno/opengl/src/textures/halo-infinite.jpg", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, true);
    
    Shader shaderCube("/home/arno/opengl/src/shader/vertex/vertexCube.glsl", "/home/arno/opengl/src/shader/fragment/fragCube.glsl");
    Shader shaderLamp("/home/arno/opengl/src/shader/vertex/vertexLight.glsl", "/home/arno/opengl/src/shader/fragment/fragLamp.glsl");

    glClearColor(0.f, 0.3f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST);// ! don't forget the z-buffer
    int width;
    int height;
    glfwGetWindowSize(window, &width, &height);

    // * Model matrix : translations, scaling, rotations
    mat4 trans;
    trans = translate(trans, vec3(-0.5f, 0.f, 0.f));

    // * View matrix : doesn't really moves the camera, it moves the entire scene
    mat4 view;
    view = translate(view, vec3(0.f, 0.f, -4.f));


    // * Projections matrix : frustums definitions
    mat4 orth = ortho(0.f, 600.f, 0.f, 800.f, 0.1f, 100.f);
    mat4 persp = perspective(radians(80.f), (float)width / (float)height, 0.1f, 100.f);

    vec3 cameraPos  = vec3(0.f, 0.f, 5.f);
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
    shaderCube.setVec3("directional.ambient", vec3(0.1f,0.1f,0.1f));
    shaderCube.setVec3("directional.diffuse", vec3(1,0,1));
    shaderCube.setVec3("directional.specular", vec3(1,1,1));

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
        vec3(0,0,0),
        vec3(0, 1, 0),
        vec3(0, 0, 1),
        vec3(0, -1, 0),
        vec3(0,0, -1),
    };

    // Moar lights !
    //--------------
    vec3 pointLightPositions[] = {
	    vec3( 0.7f,  0.2f,  2.0f),
	    vec3( 2.3f, -3.3f, -4.0f),
	    vec3(-4.0f,  2.0f, -4.0f),
	    vec3( 0.0f,  3.0f, -3.0f)
    };

    shaderCube.use();
    for (int i=0; i < 4; i++)
    {
        std::string p1 = "pointLights[";
        std::string s = std::to_string(i);
        std::string p2 = "].";
        shaderCube.setVec3(p1 + s + p2 + "position", 4.0f * pointLightPositions[i]);
        shaderCube.setVec3(p1 + s + p2 + "ambient", vec3(0.1f,0.1f,0.1f));
        shaderCube.setVec3(p1 + s + p2 + "diffuse", vec3(1,1,1));
        shaderCube.setVec3(p1 + s + p2 + "specular", vec3(1,1,1));
        shaderCube.setFloat(p1 + s + p2 + "kC", 1);
        shaderCube.setFloat(p1 + s + p2 + "kL", 0.00f);
        shaderCube.setFloat(p1 + s + p2 + "kQ", 0.000f);
    }
    //*/
    // ! !!!!!!!!Shader shaderNanosuit("/home/arno/opengl/src/shader/vertex/vertexCube.glsl", "/home/arno/opengl/src/shader/fragment/nanosuit.glsl");
    //*/
    //char* nanopath = "/home/arno/opengl/src/LoadedObjects/textures/scene.fbx";
    //Model nanosuit = Model(nanopath);
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        camera_input(window, &cam);
        process_input(window, 50000.f, &lightTranslation);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetWindowSize(window, &width, &height);

        // Cube
        //-----
        shaderCube.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, container.textID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, containerSpecular.textID);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, emissionMap.textID);
        mat4 camMat = cam.GetViewMatrix();
        shaderCube.setMat4("view", camMat);
        shaderCube.setVec3("point.position", lightTranslation);
        shaderCube.setVec3("camPos", cam.position);
        shaderCube.setVec3("camForward", cam.front);
        for(int i = 0; i < sizeof(cubePositions) / sizeof(*cubePositions); i++)
        {
            mat4 scale3;
            scale3 = scale(scale3, vec3(4, 4, 4));
            mat4 pos;
            pos = translate(pos, 3.0f * cubePositions[i]);
            mat4 rot;
            rot = rotate(rot, (float)radians(glfwGetTime() / (i + 1)) * 20.f, vec3(1, 1, i));
            shaderCube.setMat4("model", scale3 * pos * rot);
            glBindVertexArray(VAO1);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));
        }

        // Lamp
        //-----
        shaderLamp.use();
        shaderLamp.setMat4("view", camMat);
        for (int i=0; i < 4; i++)
        {
            mat4 trans;
            trans = translate(trans, pointLightPositions[i]);
            shaderLamp.setMat4("model", trans);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));
        }//*/

        /*shaderNanosuit.use();
        shaderNanosuit.setMat4("view", camMat);
        mat4 scal;
        scal = scale(scal, vec3(0.2f, 0.2f, 0.2f));
        shaderNanosuit.setMat4("model", scal);
        //nanosuit.Draw(shaderNanosuit);//*/

        glfwSwapBuffers(window);
    }//*/


    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{   
    (void)window;
    static float x = xpos;
    static float y = ypos;
    
    cam.ProcessMouseMouvement(xpos - x, y - ypos);
    x = xpos;
    y = ypos;
}