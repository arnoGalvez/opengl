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
#define SCR_WIDTH 900
#define SCR_HEIGHT 900

using namespace glm;

int main()
{
	// SECTION  Window Initialisation
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Noise Test", NULL, NULL);

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

	// SECTION  Objects Initialisation
	glEnable(GL_DEPTH_TEST);
	float radius = 6.f; // ANCHOR camera radius
	mat4 persp;
	persp = perspective(radians(70.0f), (float)(SCR_HEIGHT) / (float)(SCR_WIDTH), 0.5f, 100.f);
	mat4 view;
	mat4 model;
	model = scale(model, vec3(1, 1, 1));
	Shader shaderCube("/home/arno/Proj/opengl/src/shader/vertex/noise.vs.glsl", "/home/arno/Proj/opengl/src/shader/fragment/noise.fs.glsl");
	// Shader shaderNormal("/home/arno/Proj/opengl/src/shader/vertex/noise.vs.glsl", "/home/arno/Proj/opengl/src/shader/geometry/geometry.gs.glsl", "/home/arno/Proj/opengl/src/shader/fragment/noise.fs.glsl");
	shaderCube.use();///home/arno/Proj/opengl/src/shader/vertex/geometry.glsl
	shaderCube.setMat4("persp", persp);
	shaderCube.setMat4("model", model);
	shaderCube.setVec3("color", vec3(0, 1, 0));
	// shaderNormal.use();
	// shaderNormal.setMat4("persp", persp);
	// shaderNormal.setMat4("model", model);
	// shaderNormal.setVec3("color", vec3(1, 0, 0));

	// SECTION Cube
	Cube *cube = new Cube();
	// !SECTION

	// SECTION Positions in a cubic grid, 3 cube per side
	unsigned int cubicPositionsSidePoints = 3;
	Position *positions = new Position(cubicPositionsSidePoints, 5.0f);
	UBO *uboPositions = new UBO(0, cubicPositionsSidePoints * cubicPositionsSidePoints * cubicPositionsSidePoints, GL_DYNAMIC_DRAW); // FIXME wrong size ! * sizeof(int) !
	uboPositions->FillMat4(positions->GetPtr(), 0, positions->GetMatricesCount());
	shaderCube.use();
	shaderCube.bindToUbo("Positions", 0);
	// !SECTION

	// SECTION Framebuffer
	Framebuffer framebuffer(SCR_WIDTH, SCR_HEIGHT);
	Shader shaderFrame("/home/arno/Proj/opengl/src/shader/vertex/frameBuffer.glsl", "/home/arno/Proj/opengl/src/shader/fragment/frameBuffer.glsl");
	shaderFrame.setInt("screen", 0);
	// !SECTION

	// SECTION Spiral
	float amplitudeSpeed = 1.0f;
	float angularSpeed = 100.0f;
	// !SECTION

	// SECTION Noise
	GLsizei noiseDimension = 256;
	Noise noise(noiseDimension, 345642552);
	shaderCube.use();

	float amplitude = 1.f;
	float frequency = .06f;
	float tempAmplitude = amplitude;
	float tempFrequency = frequency;

	for (size_t i = 0; i < 8; i++)
	{
		std::string s = "noise[" + std::to_string(i) + "].";
		shaderCube.setInt(s + "texture", 0);
		shaderCube.setInt(s + "width", noiseDimension);
		shaderCube.setInt(s + "height", noiseDimension);
		shaderCube.setFloat(s + "amplitude", tempAmplitude);
		shaderCube.setFloat(s + "frequency", tempFrequency);
		tempAmplitude *= .35f;
		tempFrequency *= 1.8f;
	}
	// UBO *uboArrays = new UBO(1, noise.GetPermutationTableSize() * sizeof(int), GL_STATIC_DRAW);
	// uboArrays->Fill(noise.GetPermutationTable(), 0, noise.GetPermutationTableSize() * sizeof(int));
	// uboArrays->PrintIntBufferContent();
	// shaderCube.bindToUbo("Arrays", 1);
	
	// int *p = noise.GetPermutationTable();
	// for(size_t i = 0; i < noise.GetPermutationTableSize(); i++)
	// {
	// 	std::string s = "table[" + std::to_string(i) + "]";
	// 	shaderCube.setInt(s, p[i]);
	// }
	
	int INDEX = 0;
	shaderCube.setInt("INDEX", INDEX);
	// !SECTION
	// !SECTION

	// SECTION  main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		camera_input(window, NULL);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			amplitude += 0.01f;
			// amplitudeSpeed += 0.001f;
			/*INDEX = (INDEX + 1) % (noiseDimension * 2);
			shaderCube.use();
			shaderCube.setInt("INDEX", INDEX);
			std::cout << "INDEX:" << INDEX << std::endl;*/
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			amplitude -= 0.01f;
			// amplitudeSpeed -= 0.001f;
			/*if (INDEX == 1)
				INDEX = 0;
			else
				INDEX = (INDEX - 1) % (noiseDimension * 2);
			shaderCube.use();
			shaderCube.setInt("INDEX", INDEX);
			std::cout << "INDEX:" << INDEX << std::endl;*/
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			angularSpeed -= 0.5f;
			frequency -= 0.001f;
			if (frequency < 0.01f)
				frequency = 0.01f;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			angularSpeed += 0.5f;
			frequency += 0.001f;
		}
		float time = 0.0f * glfwGetTime(); // ANCHOR timer
		shaderCube.use();
		// shaderCube.setFloat("spiral.amplitudeSpeed", amplitudeSpeed);
		// shaderCube.setFloat("spiral.angularSpeed", angularSpeed);
		float tempAmplitude = amplitude;
		float tempFrequency = frequency;

		shaderCube.setFloat("time", time);
		for (size_t i = 0; i < 8; i++)
		{
			std::string s = "noise[" + std::to_string(i) + "].";
			shaderCube.setFloat(s + "amplitude", tempAmplitude);
			shaderCube.setFloat(s + "frequency", tempFrequency);
			tempAmplitude *= .35f;
			tempFrequency *= 1.8f;
		}

		// shaderCube.setFloat("noise.frequency", amplitudeSpeed);
		// shaderCube.setFloat("noise.amplitude", angularSpeed / 100.f);
		glActiveTexture(GL_TEXTURE0); // ANCHOR noise use
		noise.BindNoise();

		framebuffer.Bind();
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(cube->GetVAO());
		float x = -cos(0.3f * time) * radius;
		float y = x;
		float z = sin(0.3f * time) * radius;
		view = lookAt(vec3(x, y, z), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		shaderCube.use();
		shaderCube.setMat4("view", view);
		glDrawArraysInstanced(GL_TRIANGLES, 0, cube->GetVerticesCount(), cubicPositionsSidePoints * cubicPositionsSidePoints * cubicPositionsSidePoints);
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
