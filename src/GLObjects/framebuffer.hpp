#ifndef __FRAMEBUFFER_H
#define __FRAMEBUFFER_H
#include <glad/glad.h>
#include <iostream>
#include "shader/shader.h"

class Framebuffer {
    
    public:
        Framebuffer(GLsizei bufferWidth, GLsizei bufferHeight)
        {
            attachments = new GLuint[NumAttachments];
            GenFramebuffer();
            glBindFramebuffer(GL_FRAMEBUFFER, attachments[Frame]);
            GenColorBuffer(bufferWidth, bufferHeight);
            AttachTextureComponent(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, attachments[Color]);
            GenDepthStencilRenderbuffer(bufferWidth, bufferHeight);
            AttachRenderbufferComponent(GL_DEPTH_STENCIL_ATTACHMENT, attachments[DepthAndStencil]);
            CheckCompletness();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            GenScreenVAO();
        }
        ~Framebuffer()
        {
            /*glDeleteFramebuffers(1, attachments + Frame);
            glDeleteTextures(1, attachments + Color);
            glDeleteRenderbuffers(1, attachments + DepthAndStencil);*/
            delete attachments;
            delete buffers;
        }

        GLuint GetColorBufferId()
        {
            return attachments[Color];
        }

        void Bind()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, attachments[Frame]);
        }

        void Draw(Shader* shader, GLenum textureLocation)
        {
            glActiveTexture(textureLocation);
            glBindTexture(GL_TEXTURE_2D, GetColorBufferId());
            shader->use();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }


    private:
        GLsizei width, height;
        GLuint VAO;
        enum {Frame, Color, DepthAndStencil, NumAttachments};
        GLuint *attachments;
        enum {Vbo, Ebo, NumBuffers};
        GLuint *buffers;
        

        void GenFramebuffer()
        {
            glGenFramebuffers(1, attachments + Frame);
        }

        void GenColorBuffer(GLsizei bufferWidth, GLsizei bufferHeight)
        {
            glGenTextures(1, attachments + Color);
            glBindTexture(GL_TEXTURE_2D, attachments[Color]);
            float borderColor[] = {0.0f, 0.0f, 0.3f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_CLAMP_TO_BORDER, borderColor);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bufferWidth, bufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        }

        void GenDepthStencilRenderbuffer(GLsizei bufferWidth, GLsizei bufferHeight)
        {
            glGenRenderbuffers(1, attachments + DepthAndStencil);
            glBindRenderbuffer(GL_RENDERBUFFER, attachments[DepthAndStencil]);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, bufferWidth, bufferHeight);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }

        void AttachTextureComponent(GLenum colorNumber, GLenum type, GLint textureId)
        {
            if (type == GL_TEXTURE_1D)
                glFramebufferTexture1D(GL_FRAMEBUFFER, colorNumber, type, textureId, 0);
            else if (type == GL_TEXTURE_2D)
                glFramebufferTexture2D(GL_FRAMEBUFFER, colorNumber, type, textureId, 0);
        }

        void AttachRenderbufferComponent(GLenum type, GLint renderbufferId)
        {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, type, GL_RENDERBUFFER, renderbufferId);
        }

        void CheckCompletness()
        {
            GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            std::string error = "ERROR::FRAMEBUFFER \n ";
            if (status == GL_FRAMEBUFFER_COMPLETE) return;
            else if (status == GL_FRAMEBUFFER_UNDEFINED)
                std::cout << error + "default framebuffer doesn't exist" << std::endl;
            else if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
                std::cout << error + "an attachment in uninitialized" << std::endl;
            else if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
                std::cout << error + "no image attachment" << std::endl;
            else if (status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
                std::cout << error + "not every draw buffer has an attachment" << std::endl;
            else if (status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
                std::cout << error + "no read buffer" << std::endl;
            else if (status == GL_FRAMEBUFFER_UNSUPPORTED)
                std::cout << error + "framebuffer not supported on this platform" << std::endl;
            else if (status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
                std::cout << error + "the number of samples for all images across the framebuffer's attachment do not match" << std::endl;
            else
                std::cout << error << std::endl;
            
        }

        void GenScreenVAO()
        {
            float vertices[] = {
                -1.0f, -1.0f, 0.0f, 0.0f,// position (NDC) & texture coordinates
                1.0f, -1.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 0.0f, 1.0f,
            };

            GLuint indices[] = {
                0,1,2,
                2,0,3,
            };
            buffers = new GLuint[NumBuffers];
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            glGenBuffers(NumBuffers, buffers);
            glBindBuffer(GL_ARRAY_BUFFER, buffers[Vbo]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Ebo]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            GLsizei stride = 4 * sizeof(float);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, 0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));

            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Ebo]);
            /*glBindBuffer(GL_ARRAY_BUFFER, buffers[Vbo]);
            float *ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
            for (int i = 0; i < 4 * 4; i++)
            {
                std::cout << ptr[i] << std::endl;
            }
            std::cout << "...done" << std::endl;
            glUnmapBuffer(GL_ARRAY_BUFFER);*/
            glBindVertexArray(0);
        }
};

#endif // !__FRAMEBUFFER_H