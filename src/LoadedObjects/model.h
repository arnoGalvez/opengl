#ifndef __MODEL_H
#define __MODEL_H

#include <glad/glad.h> 

#include <vector>
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader/shader.h"
#include "mesh.h"

class ImportOption
{
    public:
        unsigned int aiProcessFlags = aiProcess_Triangulate;
        GLint wrapS = GL_REPEAT;
        GLint wrapT = GL_REPEAT;
        GLint magFilter = GL_LINEAR;
        GLint minFiler = GL_LINEAR_MIPMAP_LINEAR; 
};

class Model
{
    public:
        ImportOption option;
        Model(char* path)
        {
            option = ImportOption();
            loadModel(path);
        }
        Model(char* path, ImportOption opt)
        {
            option = opt;
            loadModel(path);
        }
        void Draw(Shader shader)
        {
            for(size_t i = 0; i < meshes.size(); i++)
            {
                meshes[i].Draw(shader);
                //std::cout << meshes[i].vertices.size() << std::endl;
            }
            
        }
    private:
        //  * Props
        vector<Mesh> meshes;
        vector<Texture> textures_loaded;
        string directory;
        
        // * Functions
        void loadModel(string path)
        {
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile(path, option.aiProcessFlags);
            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
            {
                cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
                return;
            }
            directory = path.substr(0, path.find_last_of('/'));
            processNode(scene->mRootNode, scene);
        }

        void processNode(aiNode* node, const aiScene* scene)
        {
            // process all of the node's meshes
            for (size_t i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene));
            }

            // process this node's children
            for (size_t i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }
        }

        Mesh processMesh(aiMesh* mesh, const aiScene* scene)
        {
            vector<Vertex> vertices;
            vector<unsigned int> indices;
            vector<Texture> textures;

            for (size_t i = 0; i < mesh->mNumVertices; i++)
            {

                Vertex vertex;
                glm::vec3 position;
                position.x = mesh->mVertices[i].x;
                position.y = mesh->mVertices[i].y;
                position.z = mesh->mVertices[i].z;
                //Vector3DtoVec3(&mesh->mVertices[i], &position);
                vertex.Position = position;
                glm::vec3 normal;
                normal.x = mesh->mNormals[i].x;
                normal.y = mesh->mNormals[i].y;
                normal.z = mesh->mNormals[i].z;
                //Vector3DtoVec3(&(mesh->mNormals[i]), &normal);
                vertex.Normal = normal;
                if(mesh->mTextureCoords[0])
                {
                    glm::vec2 texCoord;
                    texCoord.x = mesh->mTextureCoords[0][i].x;
                    texCoord.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoords = texCoord;
                }
                else
                    vertex.TexCoords = glm::vec2(0,0);

                vertices.push_back(vertex);
            }
            
            for(size_t i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for(size_t j = 0; j < face.mNumIndices; j++)
                {
                    indices.push_back(face.mIndices[j]);
                }
            }

            
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            
            vector<Texture> diffuseMaps = loadMaterial(material, aiTextureType_DIFFUSE, diffuse);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            
            vector<Texture> specularMaps = loadMaterial(material, aiTextureType_SPECULAR, specular);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            
            vector<Texture> ambientMaps = loadMaterial(material, aiTextureType_AMBIENT, ambient);
            textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
            
            //std::cout << vertices.size() << std::endl;

            return Mesh(vertices, indices, textures);
        }
        
        vector<Texture> loadMaterial(aiMaterial* mat, aiTextureType typ, types type)
        {
            vector<Texture> textures;
            //std::cout << mat->GetTextureCount(typ) << std::endl;
            
            for (unsigned int i=0; i < mat->GetTextureCount(typ); i++)
            {
                aiString path;
                mat->GetTexture(typ, i, &path);
                Texture texture;
                texture.path = path.C_Str();// ! path is local to the model object, ie in the same dir !
                texture.type = type;
                int j = Contains(textures_loaded, texture.path);
                if (j != -1)
                {
                    texture.Id = textures_loaded[j].Id;
                    //std::cout << texture.path << std::endl;
                }
                else
                {
                    texture.Id = TextureFromFile(path.C_Str(), directory);
                    textures_loaded.push_back(texture);
                }
                textures.push_back(texture);
            }

            return textures;    
        }

        void Vector3DtoVec3(const aiVector3D* vec3D, glm::vec3* vec)
        {
            vec->x = vec3D->x;
            vec->y = vec3D->y;
            vec->z = vec3D->z;
        }

        unsigned int TextureFromFile(const char* path, const string dir, bool gamma=false)
        {
            string filename = string(path);
            string pathToTexture = dir + "/" + filename;

            stbi_set_flip_vertically_on_load(true);
            int width, height, nrChannels;
            unsigned char* data = stbi_load(pathToTexture.c_str(), &width, &height, &nrChannels, 0);

            unsigned int Id;
            int format = GL_RED;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;
            if (data)
            {
                glGenTextures(1, &Id);
                glBindTexture(GL_TEXTURE_2D, Id);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, option.wrapS);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, option.wrapT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, option.magFilter);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, option.minFiler);
                
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
                //std::cout << "Succeded to load image. Path to image:\n" << pathToTexture << std::endl;
            }
            else
            {
                std::cout << "Failed to load image. Path to image:\n" << pathToTexture << std::endl;
            }
            stbi_image_free(data);

            return Id;
        }

        int Contains(vector<Texture> vec, string texPath)
        {
            
            for(int i = 0; i < vec.size(); i++)
            {
                if (vec[i].path == texPath)
                    return i;
            }
            return -1;
        }
};

#endif
