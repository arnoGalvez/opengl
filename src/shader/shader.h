#ifndef SHADER_H
#define SHADER_H

//----------------------------------------------------------------------------------------
// This code is mostly taken from available at https://learnopengl.com/
// I only added a few methods, and refactorised some code to incorporate geometry shaders.
//----------------------------------------------------------------------------------------

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        unsigned int vertex, fragment;
        vertex = genShader(GL_VERTEX_SHADER, vertexPath);
        fragment = genShader(GL_FRAGMENT_SHADER, fragmentPath);
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    Shader(const char *vertexPath, const char *geometryPath, const char *fragmentPath)
    {
        unsigned int vertex, geometry, fragment;
        vertex = genShader(GL_VERTEX_SHADER, vertexPath);
        geometry = genShader(GL_GEOMETRY_SHADER, geometryPath);
        fragment = genShader(GL_FRAGMENT_SHADER, fragmentPath);

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, geometry);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(geometry);
        glDeleteShader(fragment);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use() 
    { 
        glUseProgram(ID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    //-------------------------------------------------------------------------
    void setVec3f(const std::string &name, float *value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value);
    }
    //-------------------------------------------------------------------------
    void setVec3(const std::string &name, glm::vec3 v)
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z);
    }
    //-------------------------------------------------------------------------
    void setVec4(const std::string &name, glm::vec4 v)
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z, v.w);
    }
    //-------------------------------------------------------------------------
    void setMat4(const std::string &name, glm::mat4 m)
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(m));
    }
    //-------------------------------------------------------------------------
    void bindToUbo(const std::string &blockName, const unsigned int blockBindingPoint)
    {
        unsigned int blockLoc = glGetUniformBlockIndex(ID, blockName.c_str());
        glUniformBlockBinding(ID, blockLoc, blockBindingPoint);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    unsigned int genShader(const GLenum shaderType, const char *path)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string shaderCode;
        std::ifstream shaderFile;
        // ensure ifstream objects can throw exceptions:
        shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            // open files
            shaderFile.open(path);
            std::stringstream shaderStream;
            // read file's buffer contents into streams
            shaderStream << shaderFile.rdbuf();
            // close file handlers
            shaderFile.close();
            // convert stream into string
            shaderCode = shaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* shaderCodeC = shaderCode.c_str();
        // 2. compile shader
        unsigned int shader;
        shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderCodeC, NULL);
        glCompileShader(shader);
        if (shaderType == GL_VERTEX_SHADER)
            checkCompileErrors(shader, "VERTEX");
        else if (shaderType == GL_FRAGMENT_SHADER)
            checkCompileErrors(shader, "FRAGMENT");
        else if (shaderType == GL_GEOMETRY_SHADER)
            checkCompileErrors(shader, "GEOMETRY");
        
        return shader;
    }
};
#endif

