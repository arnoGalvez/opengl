#include <glad/glad.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class GenTexture2D
{
    public:
        int width, height;
        unsigned int textID;
        GenTexture2D(const char* pathToImage, GLint wrap_s, GLint wrap_t, GLint mag, GLint min)
        {
            // 2 : how the function should perform outside it's range ?
            // what type of texture is being affected, 
            // along which axis
            // what behaviour should be assigned
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
            //GL_CLAMP_TO_BORDER is a bit special : we have to specify a color !
            float aColor[] = {1.f, 0.f, 0.f, 1.f};
            glTexParameterfv(GL_TEXTURE_2D, GL_CLAMP_TO_BORDER, aColor);

            // 2: how the function actually chooses which texel to return given a point (x,y) ?
            //we look were falls that coordinate on the texture, and then we have to decide what to do:
            //first, if the image we want to paint is the size of the texture, no problem, avery coordinate unimbanguously
            //corresponds to a texel.
            //else, we can choose to interpolates between the neighbooring texels, or output the nearest.

            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);

            // 3: 2 showed one thing : mapping (x,y) to a texel can be tricky.
            // Imagine an object very small tries to sample from an high res texture, retrieveing the right pixel could be costly
            // because right texel x-axis = point.x * num of texels on x-axis, and that last number could be big !
            // so: mipmaps ! a texture is regrouped with it's sisters, each one being half the size of the previous one
            // well of course you will probably paint an object not the size of any of those txtures, so we have to choose
            // a way to chose which one(s) to take : the nearest one, or a linear interpolation of the the texels of two mipmaps.
            // we choose the texels as in 2.

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);

            // 4: generating the texture. Please note UNSIGNED
            glGenTextures(1, &textID);
            glBindTexture(GL_TEXTURE_2D, textID);
            stbi_set_flip_vertically_on_load(true);
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
