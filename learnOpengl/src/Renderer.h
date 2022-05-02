#pragma once

#include <GL/glew.h>

#define ASSERT(x) if(!(x)) __debugbreak()

#define GLCall(func) \
    do { \
        GlClearError(); \
        func; \
        ASSERT(GlLogCall(#func, __FILE__, __LINE__)); \
    } while(0)

void GlClearError();
bool GlLogCall(const char* function, const char* file, int line);