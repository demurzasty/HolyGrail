#pragma once 

#include <GL/glew.h>

#include "Shader.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"

struct Vertex {
    Vector3 Position;
    Vector2 UV;
    Vector3 Normal;
};

struct ChunkFeedback {
    GLuint vertexCount = 0;
    GLuint indexCount = 0;
};

struct SubChunkFeedback {
    GLuint vertexOffset = 0;
    GLuint vertexCount = 0;
    GLuint indexOffset = 0;
    GLuint indexCount = 0;
};

class Chunk {
public:
    static constexpr unsigned int ChunkSize = 80;
    static constexpr unsigned int WorkGroupSize = 8;
    static constexpr unsigned int SubChunkSize = ChunkSize / WorkGroupSize;

public:
    Chunk();

    ~Chunk();

    void SetVoxel(unsigned int x, unsigned int y, unsigned int z, unsigned int value);

    void Render(Shader* feedbackShader, Shader* voxelizerShader, Shader* forwardShader);

    GLuint GetChunkFeedbackBufferId() const;

    GLuint GetSubChunkFeedbackBufferId() const;

    const ChunkFeedback& GetChunkFeedback() const;

    const SubChunkFeedback* GetSubChunkFeedbacks() const;

private:
    void Regenerate(Shader* feedbackShader, Shader* voxelizerShader) const;

private:
    GLuint mVoxelBufferId = 0;
    unsigned int* mVoxels = nullptr;
    GLuint mChunkFeedbackBufferId = 0;
    GLuint mSubChunkFeedbackBufferId = 0;
    mutable ChunkFeedback mChunkFeedback;
    mutable SubChunkFeedback* mSubChunkFeedbacks = 0;

    mutable GLuint mVertexArrayObjectId = 0;
    mutable GLuint mVertexBufferId = 0;
    mutable GLuint mIndexBufferId = 0;

    mutable bool mDirty = false;
};
