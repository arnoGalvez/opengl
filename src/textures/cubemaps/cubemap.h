#ifndef __CUBEMAMP_H
#define __CUBEMAMP_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "camera/camera.h"
#include "shader/shader.h"

#define BUFF_OFFSET(offset) ((void *)(offset))
#define STRIDE(n, type) (n * (sizeof(type)))

using namespace std;
using namespace glm;

class Cubemap
{
  public:
	unsigned int Id;
	Cubemap(vector<string> textures, int arrayElementsCount, const float *cubeVertices, const mat4 *proj)
	{
		for (size_t i = 0; i < arrayElementsCount; i++)
		{
			cube[i] = cubeVertices[i];
		}
		Initialize(textures, arrayElementsCount);
		shader = new Shader("/home/arno/opengl/src/shader/vertex/cubemap.glsl", "/home/arno/opengl/src/shader/fragment/cubemap.glsl");
		mat4 scaleFactor;
		scaleFactor = scale(scaleFactor, vec3(5, 5, 5));
		shader->use();
		shader->setMat4("model", scaleFactor);
		shader->setMat4("proj", *proj);
		shader->setInt("skybox", 0);
	}
	~Cubemap()
	{
		delete shader;
	}
	void Draw(const mat4 *view)
	{
		mat4 viewWithoutTranslation = mat4(mat3(*view));
		shader->use();
		shader->setMat4("view", viewWithoutTranslation);
		glDepthFunc(GL_LEQUAL);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Id);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);
	}

  private:
	unsigned int VAO, VBO;
	float cube[8 * 36];
	Shader *shader;

	void Initialize(vector<string> textures, int arrayElementsCount)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glGenTextures(1, &Id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Id);
		int width, height, nrChannels;
		unsigned char *data;
		stbi_set_flip_vertically_on_load(false);
		for (GLuint i = 0; i < textures.size(); i++)
		{
			data = stbi_load(textures[i].c_str(), &width, &height, &nrChannels, 0);
			GLenum format = GL_RGB;
			if (nrChannels == 4)
				format = GL_RGBA;
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
				stbi_image_free(data);
			}
			else
			{
				cout << "failed to load image at path " << textures[i] << endl;
			}
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		InitVertices(arrayElementsCount);
	}
	void InitVertices(int arrayElementsCount)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, arrayElementsCount * sizeof(float), cube, GL_STATIC_DRAW); // ! sizeof ????
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE(3, float), BUFF_OFFSET(0));
		glBindVertexArray(0);
	}
};

#endif