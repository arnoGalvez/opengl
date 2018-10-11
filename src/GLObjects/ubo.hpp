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
        glBindBuffer(GL_UNIFORM_BUFFER, id);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, dataSize, data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    void FillMat4(const mat4 *matrices, const GLintptr offset, const GLuint matricesCount)
    {
        (void)offset;
        glBindBuffer(GL_UNIFORM_BUFFER, id);
        GLvoid* ptr = std::malloc(matricesCount * sizeof(glm::mat4));
        for(size_t i = 0; i < matricesCount; i++)
            memcpy((glm::mat4*)ptr + i, value_ptr(matrices[i]), sizeof(glm::mat4));
        glBufferData(GL_UNIFORM_BUFFER, matricesCount * sizeof(glm::mat4), ptr, GL_STATIC_DRAW);
        std::free(ptr);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    GLuint GetBindingPoint()
    {
        return bindingPoint;
    }
    ~UBO()
    {
        glDeleteBuffers(1, &id);
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
};