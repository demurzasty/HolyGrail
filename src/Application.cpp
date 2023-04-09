#include "Application.hpp"
#include "Math.hpp"

#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void OnWindowResize(GLFWwindow* window, int width, int height) {
	((Application*)glfwGetWindowUserPointer(window))->OnWindowResize(width, height);
}

void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param) {
	auto const srcStr = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		default: return "UNDEFINED";
		}
	}();

	auto const typeStr = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		default: return "UNDEFINED";
		}
	}();

	auto const severityStr = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		default: return "UNDEFINED";
		}
	}();

	printf("%s, %s, %s, %u: %s\n", srcStr, typeStr, severityStr, id, message);
}

Application::Application() {
	glfwInit();

	mWindow = glfwCreateWindow(1280, 720, "TheHolyGrail", nullptr, nullptr);

	glfwSetWindowUserPointer(mWindow, this);

	glfwMakeContextCurrent(mWindow);
	
	glfwSetWindowSizeCallback(mWindow, &::OnWindowResize);

	glewInit();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(&MessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

	mFeedbackShader = new Shader("data/feedback.comp");
	mVoxelizerShader = new Shader("data/voxelizer.comp");
	mForwardShader = new Shader("data/forward.vert", "data/forward.frag");

	mChunk = new Chunk();

	mGlobalData.View = Matrix4::Invert(Matrix4::CreateTranslation(Vector3(40.0f, 40.0f, 170.0f)));
	mGlobalData.Projection = Matrix4::CreatePerspective(Math::DegreesToRadians(45.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);

	glCreateBuffers(1, &mGlobalDataBufferId);
	glNamedBufferStorage(mGlobalDataBufferId, sizeof(GlobalData), &mGlobalData, GL_DYNAMIC_STORAGE_BIT);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, mGlobalDataBufferId);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	srand((unsigned int)time(NULL));

	for (unsigned int x = 0; x < Chunk::ChunkSize; ++x) {
		for (unsigned int y = 0; y < Chunk::ChunkSize; ++y) {
			for (unsigned int z = 0; z < Chunk::ChunkSize; ++z) {
				mChunk->SetVoxel(x, y, z, rand() % 2);
			}
		}
	}

	mLastTime = glfwGetTime();

	glfwSwapInterval(0);
}

Application::~Application() {
	glDeleteBuffers(1, &mGlobalDataBufferId);

	delete mChunk;

	delete mForwardShader;
	delete mVoxelizerShader;
	delete mFeedbackShader;

	glfwTerminate();
}

void Application::Run() {
	while (!glfwWindowShouldClose(mWindow)) {
		glfwPollEvents();

		double currentTime = glfwGetTime();
		if (currentTime > mLastTime + 0.05) {
			for (unsigned int x = 0; x < Chunk::ChunkSize; ++x) {
				for (unsigned int y = 0; y < Chunk::ChunkSize; ++y) {
					for (unsigned int z = 0; z < Chunk::ChunkSize; ++z) {
						mChunk->SetVoxel(x, y, z, rand() % 2);
					}
				}
			}

			mLastTime = currentTime;
		}

		glNamedBufferSubData(mGlobalDataBufferId, 0, sizeof(GlobalData), &mGlobalData);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mChunk->Render(mFeedbackShader, mVoxelizerShader, mForwardShader);

		glfwSwapBuffers(mWindow);
	}
}

void Application::OnWindowResize(int width, int height) {
	glViewport(0, 0, width, height);

	mGlobalData.Projection = Matrix4::CreatePerspective(Math::DegreesToRadians(45.0f), (float)width / height, 0.1f, 1000.0f);
}

void Application::CreatePerspectiveMatrix(float fovy, float aspect, float zNear, float zFar, float* output) {
	float r = fovy / 2.0f;
	float delta = zNear - zFar;
	float s = sinf(fovy);

	if (delta == 0 || s == 0 || aspect == 0) {
		return;
	}

	float cotangent = cosf(r) / s;

	float temp[16] = {
		cotangent / aspect, 0, 0, 0,
		0, cotangent, 0, 0,
		0, 0, (zNear + zFar) / delta, -1,
		0, 0, (2 * zNear * zFar) / delta, 0
	};

	memcpy(output, temp, sizeof(temp));
}
