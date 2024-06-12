#pragma once
#include <glad/glad.h>

class EBO{
    private:
        GLuint id;
    public:
        EBO(GLuint* indices, GLuint size);

        void bind();
        void unbind();
        void Delete();
};