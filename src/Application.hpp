#pragma once 

#include "Shader.hpp"
#include "Texture.hpp"
#include "Chunk.hpp"
#include "Matrix4.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct GlobalData {
    Matrix4 Projection;
    Matrix4 View;
};

class Application {
public:
    Application();

    ~Application();

    void Run();

    void OnWindowResize(int width, int height);

private:
    void CreatePerspectiveMatrix(float fovy, float aspect, float zNear, float zFar, float* output);

private:
    GLFWwindow* mWindow = nullptr;
    Shader* mFeedbackShader = nullptr;
    Shader* mVoxelizerShader = nullptr;
    Shader* mForwardShader = nullptr;
    Chunk* mChunk = nullptr;
    GlobalData mGlobalData;
    GLuint mGlobalDataBufferId = 0;
    double mLastTime = 0.0;
};
