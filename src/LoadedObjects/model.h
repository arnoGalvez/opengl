#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
#include "mesh.h"


class Model
{
    public:
        Model(char* path)
        {
            loadModel(path);
        }
        void Draw(Shader shader)
        {
            for(size_t i = 0; i < meshes.size(); i++)
            {
                meshes[i].Draw(shader);
            }
            
        }
    private:
        //  * Props
        vector<Mesh> meshes;
        string directory;
        
        // * Functions
        void loadModel(string path)
        {
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
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
                //position.x = mesh->mVertices[i].x;
                //position.y = mesh->mVertices[i].y;
                //position.z = mesh->mVertices[i].z;
                Vector3DtoVec3(&mesh->mVertices[i], &position);
                vertex.Position = position;
                glm::vec3 normal;
                //normal.x = mesh->mNormals[i].x;
                //normal.y = mesh->mNormals[i].y;
                //normal.z = mesh->mNormals[i].z;
                Vector3DtoVec3(&(mesh->mNormals[i]), &normal);
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

            }
        }

        void Vector3DtoVec3(const aiVector3D* vec3D, glm::vec3* vec)
        {
            vec->x = vec3D->x;
            vec->y = vec3D->y;
            vec->z = vec3D->z;

        }




};