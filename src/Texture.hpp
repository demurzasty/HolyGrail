#pragma once 

#include <GL/glew.h>

class Texture {
public:
    Texture(const char* filename);

    ~Texture();

    GLuint GetId() const;

    GLuint64 GetHandle() const;

private:
    GLuint mId = 0;
    GLuint64 mHandle = 0;
};
