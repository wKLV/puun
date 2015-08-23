#ifndef gl_helph
#define gl_helph

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum dataStructure {
    One,
    Vec2,
    Vec3,
    Vec4,
    M2,
    M3,
    M4,
    Texture
};

typedef struct {
    u8* name;
    enum dataStructure dataStructure;
    GLenum dataType;
    void* data;
    int texnum;
    GLuint id;
} UniformData;

void show_info_log(
            GLuint object,
            PFNGLGETSHADERIVPROC glGet__iv,
            PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
        );

GLuint setupProgram(u8* vertexSource, int vertexSourceSize, u8* fragmentSource, int fragmentSourceSize);
GLuint setupTexture_Any(void* data, int x, int y, GLenum internalFormat, GLint format, GLenum type);
GLuint setupTexture(void* data, int x, int y);
void setParam(GLsizei program, UniformData data);

#endif
