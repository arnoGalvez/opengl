#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "window/window.h"
#include "shader/shader.h"
#include "GLObjects/framebuffer.hpp"
#include "GLObjects/ubo.hpp"
#include "drawing/cube.hpp"
#include "drawing/surface.hpp"
#include "textures/import.h"
#include "drawing/position.hpp"
#include "textures/procedural/noise.hpp"
#define SCR_WIDTH 800
#define SCR_HEIGHT 800

using namespace glm;

int main()
{
	// SECTION  Initialisation
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Je suis un sanglier", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "ERROR::WINDOW IS NULL" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, framebuffer_size_callback);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	// !SECTION

	// SECTION  Stuff
	mat4 persp;
	persp = perspective(radians(70.0f), (float)(SCR_HEIGHT) / (float)(SCR_WIDTH), 0.5f, 100.f);
	mat4 view;
	mat4 model;
	model = scale(model, vec3(1, 1, 1));
	Shader shaderCube("/home/arno/opengl/src/shader/vertex/geometry.glsl", "/home/arno/opengl/src/shader/fragment/geometry.glsl");
	Shader shaderNormal("/home/arno/opengl/src/shader/vertex/geometry.glsl", "/home/arno/opengl/src/shader/geometry/geometry.glsl", "/home/arno/opengl/src/shader/fragment/geometryNormal.glsl");
	shaderCube.use();
	shaderCube.setMat4("persp", persp);
	shaderCube.setMat4("model", model);
	shaderCube.setVec3("color", vec3(0, 1, 0));
	shaderNormal.use();
	shaderNormal.setMat4("persp", persp);
	shaderNormal.setMat4("model", model);
	shaderNormal.setVec3("color", vec3(1, 0, 0));

	Cube *cube = new Cube();

	unsigned int cubeSidePoints = 3;
	Position *positions = new Position(cubeSidePoints, 5.0f);
	UBO *uboPositions = new UBO(0, cubeSidePoints * cubeSidePoints * cubeSidePoints, GL_DYNAMIC_DRAW);
	uboPositions->FillMat4(positions->GetPtr(), 0, positions->GetMatricesCount());
	shaderCube.use();
	shaderCube.bindToUbo("Positions", 0);

	Framebuffer framebuffer(SCR_WIDTH, SCR_HEIGHT);
	Shader shaderFrame("/home/arno/opengl/src/shader/vertex/frameBuffer.glsl", "/home/arno/opengl/src/shader/fragment/frameBuffer.glsl");
	shaderFrame.setInt("screen", 0);

	float radius = 4.0f; // ANCHOR radius
	float amplitudeSpeed = 1.0f;
	float angularSpeed = 100.0f;
	glEnable(GL_DEPTH_TEST);

	GLsizei noiseDimension = 16;
	Noise noise(noiseDimension, noiseDimension);
	shaderCube.use();

	float amplitude = .5f;
	float frequency = 1.0f;
	for (size_t i = 0; i < 8; i++)
	{
		std::string s = "noise[" + std::to_string(i) + "].";
		shaderCube.setInt(s + "texture", 0);
		shaderCube.setInt(s + "width", noiseDimension);
		shaderCube.setInt(s + "height", noiseDimension);
		shaderCube.setFloat(s + "amplitude", amplitude);
		shaderCube.setFloat(s + "frequency", frequency);
		amplitude /= 2.0f;
		frequency *= 2.0f;
	}

	// !SECTION

	// SECTION  main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		camera_input(window, NULL);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			amplitudeSpeed += 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			amplitudeSpeed -= 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			angularSpeed -= 0.5f;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			angularSpeed += 0.5f;
		}
		shaderCube.use();
		// shaderCube.setFloat("spiral.amplitudeSpeed", amplitudeSpeed);
		// shaderCube.setFloat("spiral.angularSpeed", angularSpeed);
		shaderCube.setFloat("time", glfwGetTime());
		// shaderCube.setFloat("noise.frequency", amplitudeSpeed);
		// shaderCube.setFloat("noise.amplitude", angularSpeed / 100.f);
		glActiveTexture(GL_TEXTURE0); // ANCHOR noise use
		noise.BindNoise();

		framebuffer.Bind();
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(cube->GetVAO());
		float t = 0.f * glfwGetTime(); // ANCHOR timer
		float x = -cos(t) * radius;
		float y = 0;
		float z = sin(t) * radius;
		view = lookAt(vec3(x, y, z), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		shaderCube.use();
		shaderCube.setMat4("view", view);
		glDrawArraysInstanced(GL_TRIANGLES, 0, cube->GetVerticesCount(), cubeSidePoints * cubeSidePoints * cubeSidePoints);
		/*shaderNormal.use();
        shaderNormal.setMat4("view", view);
        glDrawArrays(GL_TRIANGLES, 0, cube->GetVerticesCount());//*/

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.f, 0.f, 0.f, 1.0f),
			glClear(GL_COLOR_BUFFER_BIT);
		framebuffer.Draw(&shaderFrame, GL_TEXTURE0);
		glfwSwapBuffers(window);
	}

	// !SECTION

	glfwTerminate();

	return 0;
}
