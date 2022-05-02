#pragma once

#include <GL/glew.h>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak()

#define GLCall(func) \
    do { \
        GlClearError(); \
        func; \
        ASSERT(GlLogCall(#func, __FILE__, __LINE__)); \
    } while(0)

void GlClearError();
bool GlLogCall(const char* function, const char* file, int line);

class Renderer {
private:

public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};