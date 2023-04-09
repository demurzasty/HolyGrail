#include "Shader.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

Shader::Shader(const char* computeShaderFilename) {
    char* shaderCode = ReadAllText(computeShaderFilename);

    CompileComputeShader(shaderCode);

    free(shaderCode);
}

Shader::Shader(const char* vertexShaderFilename, const char* fragmentShaderFilename) {
    char* vertexCode = ReadAllText(vertexShaderFilename);
    char* fragmentCode = ReadAllText(fragmentShaderFilename);

    CompileStandardShader(vertexCode, fragmentCode);

    free(fragmentCode);
    free(vertexCode);
}

Shader::~Shader() {
    glDeleteProgramPipelines(1, &mId);
}

GLuint Shader::GetId() const {
    return mId;
}

char* Shader::ReadAllText(const char* filename) {
    FILE* file = fopen(filename, "rb");
    assert(file);

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* text = (char*)malloc(size + 1);
    assert(text);

    fread(text, 1, size, file);
    text[size] = 0;

    fclose(file);
    return text;
}

void Shader::CompileStandardShader(const char* vertexShaderCode, const char* fragmentShaderCode) {
    GLuint vertexShader = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vertexShaderCode);

    GLuint fragmentShader = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragmentShaderCode);

    TestShader(vertexShader);
    TestShader(fragmentShader);

    glCreateProgramPipelines(1, &mId);
    glUseProgramStages(mId, GL_VERTEX_SHADER_BIT, vertexShader);
    glUseProgramStages(mId, GL_FRAGMENT_SHADER_BIT, fragmentShader);
}

void Shader::CompileComputeShader(const char* computeShaderCode) {
    GLuint computeShader = glCreateShaderProgramv(GL_COMPUTE_SHADER, 1, &computeShaderCode);

    TestShader(computeShader);

    glCreateProgramPipelines(1, &mId);
    glUseProgramStages(mId, GL_COMPUTE_SHADER_BIT, computeShader);
}

void Shader::TestShader(GLuint shaderId) {
    GLint result, infoLength;
    glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
    glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &infoLength);
    if (!result && infoLength > 0) {
        char buffer[1024];
        glGetProgramInfoLog(shaderId, 1023, NULL, buffer);

        printf("%s\n", buffer);
    }
}
