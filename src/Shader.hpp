#pragma once 

#include <GL/glew.h>

class Shader {
public:
    Shader(const char* computeShaderCode);

    Shader(const char* vertexShaderCode, const char* fragmentShaderCode);

    ~Shader();

    GLuint GetId() const;

private:
    char* ReadAllText(const char* filename);

    void CompileStandardShader(const char* vertexShaderCode, const char* fragmentShaderCode);

    void CompileComputeShader(const char* computeShaderCode);

    void TestShader(GLuint shaderId);

private:
    GLuint mId = 0;
};
