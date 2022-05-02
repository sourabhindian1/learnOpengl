#include "Renderer.h"

#include <iostream>
#include <format>

void GlClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) {
        std::cout << std::format("Opengl Error: {} Function: {} File: {} Line: {}\n",
            error, function, file, line);
        return false;
    }
    return true;
}