#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <format>

Shader::Shader(const std::string& filename)
    : m_FilePath(filename), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(m_FilePath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
#if 0
    std::cout << std::format("\nVERTEX SHADER\n");
    std::cout << std::format("{}\n", source.VertexSource);
    std::cout << std::format("\nFRAGMENT SHADER\n");
    std::cout << std::format("{}\n", source.FragmentSource);
#endif
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(m_FilePath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    enum ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Check for compilation error
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string & vertexShader, const std::string & fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glBindAttribLocation(program, 0, "position");

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}
void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    int location = GetUniformLocation(name);
    if (location != -1)
        glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    int location = GetUniformLocation(name);
    if (location != -1)
        glUniform1i(location, value);
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) {
        std::cout << std::format("Warning: Uniform Location of {} is not found\n", name);
    }

    m_UniformLocationCache[name] = location;
    return location;
}