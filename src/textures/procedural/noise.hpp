#include <glad/glad.h>
#include <cstdlib>

class Noise
{
  public: // ANCHOR public
	Noise(GLsizei w, GLsizei h)
	{
		width = w;
		height = h;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, height, 0, GL_RED, GL_FLOAT, GenData());
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void BindNoise()
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

  private: // ANCHOR private
	GLsizei width, height;
	GLuint textureID;
	float *randomValues;
	void *GenData()
	{
		srand48(13564689);
		randomValues = new float[width * height];
		for (GLsizei i = 0; i < width * height; i++)
		{
			randomValues[i] = drand48();
		}
		return randomValues;
	}
	void FreeData()
	{
		delete randomValues;
	}
};