#include <glad/glad.h>
#include <cstdlib>


class Noise
{
  public:
	Noise(GLsizei width, long noiseSeed)//, unsigned int permutationTableSeed)
		: width(width), noiseSeed(noiseSeed)//, permutationTableSeed(permutationTableSeed)
	{
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, width, 0, GL_RED, GL_FLOAT, GenData());
		glBindTexture(GL_TEXTURE_2D, 0);
		FreeNoiseValues();
	}
	void BindNoise()
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	/*int GetPermutationTableSize()
	{
		return 2 * width;
	}
	int* GetPermutationTable()
	{
		return &permutationTable[0];
	}//*/

  private:
	GLsizei width;
	GLuint textureID;
	long noiseSeed;
	float *noiseValues;
	// unsigned int permutationTableSeed;
	// int *permutationTable;
	void* GenData()
	{
		srand48(noiseSeed);
		noiseValues = new float[width * width];
		// srand(permutationTableSeed);
		// permutationTable = new int[2 * width];
		for (GLsizei i = 0; i < width * width; i++)
		{
			noiseValues[i] = drand48();
			// permutationTable[i] = i;
		}
		// for (GLsizei i = 0; i < width * width; i++)
		// {
		// 	int k = rand() % width;
		// 	Swap(permutationTable + i, permutationTable + k);
		// 	permutationTable[i + width] = permutationTable[i];
		// }
		
		return noiseValues;
	}
	void FreeNoiseValues()
	{
		delete[] noiseValues;
	}
	void Swap(int *p1, int *p2)
	{
		int tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
	}
};