#ifndef __MESH_H
#define __MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <shader.h>
using namespace std;

#define BUFFER_OFFSET(offset) ((void*) offset)

enum types {
    diffuse=0,
    specular,
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

        /* Constructor */
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
        {
            this->vertices = vertices;
            this->textures = textures;
            this->indices = indices;
            setupMesh();
        }

        /* Functions */
        void Draw(Shader shader)
        {
            shader.use();
            string dif = "textureDiffuse";
            string spec = "textureSpecular";
            int diffuseCount = 0;
            int specularCount = 0;
            int normalCount = 0;
            int heightCount = 0;
            types last= LAST;
            int counts[last];

            for (int i=0; i < textures.size(); i++)
            {
                types enumType = textures[i].type;
                string type = TypesToString(enumType);
                if (type != "")
                {
                    shader.setInt(type + to_string(counts[enumType]++), GL_TEXTURE0 + i);
                    glEnable(GL_TEXTURE0 + i);
                    glBindTexture(GL_TEXTURE_2D, textures[i].Id);
                }
            }

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glActiveTexture(GL_TEXTURE0);
        }
    private:
        /* buffers */
        unsigned int VAO, VBO, EBO;

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
                    return "diffuse";
                case specular:
                    return "specular";
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