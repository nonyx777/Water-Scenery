#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string getFileContents(const char* filename);

class Shader{
    public:
        GLuint id;
        Shader(const char* vertexFile, const char* fragmentFile);

        void use();
        void Delete();
};