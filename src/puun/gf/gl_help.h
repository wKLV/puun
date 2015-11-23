#ifndef gl_helph
#define gl_helph

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../types.h"

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
    String name;
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

GLuint setupProgram(String vertexSource, String fragmentSource);
GLuint setupTexture_Any(void* data, int x, int y, GLenum internalFormat, GLint format, GLenum type);
GLuint setupTexture(void* data, int x, int y);
UniformData setupTexture_and_uniform(void* data, int x, int y, String name);
void setParam(GLsizei program, UniformData data);

void load_wavefront_obj(String path, u32 vertexBuffer); 

#endif
