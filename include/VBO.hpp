#pragma once
#include <glad/glad.h>

class VBO{
    private:
        GLuint id;
    public:
        VBO(GLfloat* vertices, GLuint size);

        void bind();
        void unbind();
        void Delete();
};