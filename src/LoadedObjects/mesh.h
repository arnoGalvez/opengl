#ifndef __MESH_H
#define __MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "shader/shader.h"
using namespace std;

#define BUFFER_OFFSET(offset) ((void*) offset)

enum types {
    diffuse=0,
    specular,
    ambient,
    normal,
    height,
    LAST
};

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

//Naming Convention:
//textureDiffuse0
//textureSpecular0
//textureAmbient0
//...
struct Texture {
    unsigned int Id;
    types type;
    string path;
};

class Mesh 
{
    public:
        /* data */
        vector<Vertex> vertices;
        vector<Texture> textures;
        vector<unsigned int> indices;
        string dif = "textureDiffuse";
        string spec = "textureSpecular";
        string amb = "textureAmbient";
        // ! difBegin + # + difEnd ?

        /* Constructor */
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
        {
            this->vertices = vertices;
            this->textures = textures;
            this->indices = indices;
            this->b = true;
            setupMesh();
        }

        /* Functions */
        void Draw(Shader shader)
        {
            shader.use();
            int diffuseCount = 0;
            int specularCount = 0;
            int normalCount = 0;
            int heightCount = 0;
            types last = LAST;
            int counts[last];
            for (int i = 0; i < last; i++)
            {
                counts[i] = 0;
            }
            //std::cout << "---" << std::endl;
            
            for (int i=0; i < textures.size(); i++)
            {
                types enumType = textures[i].type;
                string type = TypesToString(enumType);
                if (type != "")
                {
                    string tmp = type + to_string(counts[enumType]++);
                    //if (b)
                        //std::cout << GL_MAX_TEXTURE_IMAGE_UNITS << std::endl;
                    glActiveTexture(GL_TEXTURE0 + i);
                    shader.setInt(tmp, i);
                    glBindTexture(GL_TEXTURE_2D, textures[i].Id);
                }
            }
            //if (b)
                //std::cout << "\n" << std::endl;
            //b = false;

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glActiveTexture(GL_TEXTURE0);
        }
    private:
        /* buffers */
        unsigned int VAO, VBO, EBO;
        bool b;

        /* Functions */
        void setupMesh()
        {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);



            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

            glBindVertexArray(0);
        }

        string TypesToString(types type)
        {
            switch(type)
            {
                case diffuse:
                    return dif;
                case specular:
                    return spec;
                case ambient:
                    return amb;
                case normal:
                    return "normal";
                case height:
                    return "height";
                default:
                    return "";

            }
        }

};

#endif