#include "Texture.hpp"

#include <stb_image.h>

#include <assert.h>

Texture::Texture(const char* filename) {
    int width, height;
    stbi_uc* pixels = stbi_load(filename, &width, &height, NULL, 4);
    assert(pixels);

    glCreateTextures(GL_TEXTURE_2D, 1, &mId);

    glTextureParameteri(mId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(mId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureStorage2D(mId, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(mId, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    stbi_image_free(pixels);

    mHandle = glGetTextureHandleARB(mId);
    glMakeTextureHandleResidentARB(mHandle);
}

Texture::~Texture() {
    glDeleteTextures(1, &mId);
}

GLuint Texture::GetId() const {
    return mId;
}

GLuint64 Texture::GetHandle() const {
    return mHandle;
}
