#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>

class UBO
{
  public:
    UBO(const GLuint bindPoint, const GLsizeiptr bufferSize, const GLenum bufferUsage) : bindingPoint(bindPoint), size(bufferSize), usage(bufferUsage)
    {
        Gen();
    }
    void Fill(const void *data, const GLintptr offset, const GLsizeiptr dataSize)
    {
        glGetError();
        glBindBuffer(GL_UNIFORM_BUFFER, id);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, dataSize, data);
        // glBufferData(GL_UNIFORM_BUFFER, dataSize, data, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        GLenum errorMessage = glGetError();
        PrintError(errorMessage);
    }
    void FillMat4(const mat4 *matrices, const GLintptr offset, const GLuint matricesCount)
    {
        glGetError();
        (void)offset;
        glBindBuffer(GL_UNIFORM_BUFFER, id);
        GLvoid *ptr = std::malloc(matricesCount * sizeof(glm::mat4));
        for (size_t i = 0; i < matricesCount; i++)
            memcpy((glm::mat4 *)ptr + i, value_ptr(matrices[i]), sizeof(glm::mat4));
        glBufferData(GL_UNIFORM_BUFFER, matricesCount * sizeof(glm::mat4), ptr, usage);
        std::free(ptr);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        GLenum tmp = glGetError();
        PrintError(tmp);

    }
    GLuint GetBindingPoint()
    {
        return bindingPoint;
    }
    ~UBO()
    {
        glDeleteBuffers(1, &id);
    }
    void PrintIntBufferContent()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, id);
        int *ptr = (int*)glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_ONLY);
        std::cout << "Printing int ubo " << id << " content";
        for(size_t i = 0; i < size / sizeof(int); i++)
        {
            std::cout << ptr[i] << "\n";
        }
        std::cout << "size = " << size / sizeof(int);
        std::cout << std::endl;
        GLboolean error =  glUnmapBuffer(GL_UNIFORM_BUFFER);
        if (error == GL_FALSE)
            std::cout << "an error occured in PrintBufferContent for ubo " << id << std::endl;
    }

  private:
    GLuint bindingPoint;
    GLsizeiptr size;
    GLenum usage;
    GLuint id;
    void Gen()
    {
        glGenBuffers(1, &id);
        glBindBuffer(GL_UNIFORM_BUFFER, id);
        glBufferData(GL_UNIFORM_BUFFER, size, NULL, usage);
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, id);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void PrintError(GLenum message)
    {
        switch (message)
        {
            case GL_INVALID_ENUM:
                std::cout << "ERROR WHILE FILLING UBO AT BINDING POINT " << bindingPoint << std::endl;
                std::cout << "GL_INVALID_ENUM" << "\n" << std::endl;
                return;

            case GL_INVALID_OPERATION:
                std::cout << "ERROR WHILE FILLING UBO AT BINDING POINT " << bindingPoint << std::endl;
                std::cout << "GL_INVALID_OPERATION" << "\n" << std::endl;
                return;

            case GL_INVALID_VALUE:
                std::cout << "ERROR WHILE FILLING UBO AT BINDING POINT " << bindingPoint << std::endl;
                std::cout << "GL_INVALID_VALUE" << "\n" << std::endl;
                return;
        }
        if (message != 0) 
            std::cout << "UNKNOWN ERROR WHILE FILLING UBO AT BINDING POINT " << bindingPoint << std::endl;
    }
};