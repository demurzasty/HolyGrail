#include "Chunk.hpp"
#include "Math.hpp"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Chunk::Chunk() {
    glCreateBuffers(1, &mVoxelBufferId);
    glNamedBufferStorage(mVoxelBufferId, sizeof(unsigned int) * ChunkSize * ChunkSize * ChunkSize, 0, GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

    mVoxels = (unsigned int*)glMapNamedBuffer(mVoxelBufferId, GL_READ_WRITE);

    memset(mVoxels, 0, sizeof(unsigned int) * ChunkSize * ChunkSize * ChunkSize);

    glCreateBuffers(1, &mChunkFeedbackBufferId);
    glNamedBufferStorage(mChunkFeedbackBufferId, sizeof(ChunkFeedback), 0,
        GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

    glCreateBuffers(1, &mSubChunkFeedbackBufferId);
    glNamedBufferStorage(mSubChunkFeedbackBufferId, sizeof(SubChunkFeedback) * SubChunkSize * SubChunkSize * SubChunkSize, 0, 0);

    glCreateVertexArrays(1, &mVertexArrayObjectId);

    glEnableVertexArrayAttrib(mVertexArrayObjectId, 0);
    glEnableVertexArrayAttrib(mVertexArrayObjectId, 1);
    glEnableVertexArrayAttrib(mVertexArrayObjectId, 2);

    glVertexArrayAttribFormat(mVertexArrayObjectId, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position));
    glVertexArrayAttribFormat(mVertexArrayObjectId, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, UV));
    glVertexArrayAttribFormat(mVertexArrayObjectId, 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Normal));

    glVertexArrayAttribBinding(mVertexArrayObjectId, 0, 0);
    glVertexArrayAttribBinding(mVertexArrayObjectId, 1, 0);
    glVertexArrayAttribBinding(mVertexArrayObjectId, 2, 0);
}

Chunk::~Chunk() {
    glUnmapNamedBuffer(mVoxelBufferId);

    glDeleteBuffers(1, &mIndexBufferId);
    glDeleteBuffers(1, &mVertexBufferId);
    glDeleteVertexArrays(1, &mVertexArrayObjectId);
    glDeleteBuffers(1, &mSubChunkFeedbackBufferId);
    glDeleteBuffers(1, &mChunkFeedbackBufferId);
}

void Chunk::SetVoxel(unsigned int x, unsigned int y, unsigned int z, unsigned int value) {
    unsigned int index = x + ChunkSize * (y + ChunkSize * z);

    if (index < ChunkSize * ChunkSize * ChunkSize) {
        if (mVoxels[index] != value) {
            mVoxels[index] = value;
            mDirty = true;
        }
    }
}

void Chunk::Render(Shader* feedbackShader, Shader* voxelizerShader, Shader* forwardShader) {
    if (mDirty) {
        Regenerate(feedbackShader, voxelizerShader);
        mDirty = false;
    }

    if (mVertexArrayObjectId && mChunkFeedback.indexCount > 0) {
        glBindProgramPipeline(forwardShader->GetId());

        glBindVertexArray(mVertexArrayObjectId);

        glDrawElements(GL_TRIANGLES, mChunkFeedback.indexCount, GL_UNSIGNED_INT, NULL);
    }
}

GLuint Chunk::GetChunkFeedbackBufferId() const {
    return mChunkFeedbackBufferId;
}

GLuint Chunk::GetSubChunkFeedbackBufferId() const {
    return mSubChunkFeedbackBufferId;
}

const ChunkFeedback& Chunk::GetChunkFeedback() const {
    return mChunkFeedback;
}

const SubChunkFeedback* Chunk::GetSubChunkFeedbacks() const {
    return mSubChunkFeedbacks;
}

void Chunk::Regenerate(Shader* feedbackShader, Shader* voxelizerShader) const {
    mChunkFeedback = ChunkFeedback();

    ChunkFeedback* feedback = (ChunkFeedback*)glMapNamedBuffer(mChunkFeedbackBufferId, GL_WRITE_ONLY);
    *feedback = mChunkFeedback;
    glUnmapNamedBuffer(mChunkFeedbackBufferId);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, mVoxelBufferId);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, mChunkFeedbackBufferId);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, mSubChunkFeedbackBufferId);

    glBindProgramPipeline(feedbackShader->GetId());

    glDispatchCompute(SubChunkSize, SubChunkSize, SubChunkSize);

    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    feedback = (ChunkFeedback*)glMapNamedBuffer(mChunkFeedbackBufferId, GL_READ_ONLY);
    mChunkFeedback = *feedback;
    glUnmapNamedBuffer(mChunkFeedbackBufferId);

    int currentVertexBufferSize = 0;
    if (mVertexBufferId) {
        glGetNamedBufferParameteriv(mVertexBufferId, GL_BUFFER_SIZE, &currentVertexBufferSize);
    }

    int currentIndexBufferSize = 0;
    if (mIndexBufferId) {
        glGetNamedBufferParameteriv(mIndexBufferId, GL_BUFFER_SIZE, &currentIndexBufferSize);
    }

    unsigned int newVertexBufferSize = mChunkFeedback.vertexCount * sizeof(Vertex);
    unsigned int newIndexBufferSize = mChunkFeedback.indexCount * sizeof(GLuint);

    if (newVertexBufferSize > (unsigned int)currentVertexBufferSize) {
        glDeleteBuffers(1, &mVertexBufferId);

        glCreateBuffers(1, &mVertexBufferId);
        glNamedBufferStorage(mVertexBufferId, Math::Align(newVertexBufferSize, 32 * 1024 * 1024), 0, 0);
    }

    if (newIndexBufferSize > (unsigned int)currentIndexBufferSize) {
        glDeleteBuffers(1, &mIndexBufferId);

        glCreateBuffers(1, &mIndexBufferId);
        glNamedBufferStorage(mIndexBufferId, Math::Align(newIndexBufferSize, 32 * 1024 * 1024), 0, 0);
    }

    glVertexArrayVertexBuffer(mVertexArrayObjectId, 0, mVertexBufferId, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(mVertexArrayObjectId, mIndexBufferId);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, mVertexBufferId);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, mIndexBufferId);

    glBindProgramPipeline(voxelizerShader->GetId());

    glDispatchCompute(SubChunkSize, SubChunkSize, SubChunkSize);

    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}
