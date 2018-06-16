#include <glad/glad.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class GenTexture2D
{
    public:
        int width, height;
        unsigned int textID;
        GenTexture2D(const char* pathToImage)
        {
            glGenTextures(1, &textID);
            glBindTexture(GL_TEXTURE_2D, textID);
            data = stbi_load(pathToImage, &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cout << "Failed to load image" << std::endl;
            }
            
            stbi_image_free(data);
        }
    private:
        int nrChannels;
        unsigned char *data;
};
