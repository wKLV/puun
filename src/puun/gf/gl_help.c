#ifndef gl_helpc
#define gl_helpc
#include "gl_help.h"
#include <string.h>

void show_info_log(
            GLuint object,
            PFNGLGETSHADERIVPROC glGet__iv,
            PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
        )
{
    GLint log_length;
    u8 *log;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    log = malloc(log_length);
    glGet__InfoLog(object, log_length, NULL, (char*)log);
    fprintf(stderr, "error:\n %s\n", log);
    free(log);
}

GLuint setupProgram(u8* vertexSource, int vertexSourceSize, u8* fragmentSource, int fragmentSourceSize) {
    u8* vs = vertexSource;
    u8* fs = fragmentSource;
    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLint shader_ok;

    glShaderSource(vertexShader, 1, (const GLchar**)&vs, 0);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shader_ok);
    if(shader_ok == GL_FALSE) {
        printf("Failed to compile vertex shader\n");
      //  printf("%s", vertexSource);
        show_info_log(vertexShader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(vertexShader);
    }

    glShaderSource(fragmentShader, 1, (const GLchar**)&fs, 0);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shader_ok);

    if(!shader_ok) {
        fprintf(stderr, "Failed to compile fragment shader\n");
        show_info_log(fragmentShader, glGetShaderiv, glGetProgramInfoLog);
        glDeleteShader(fragmentShader);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &shader_ok);
    if(!shader_ok) {
        fprintf(stderr, "Failed to link shader program\n");
        show_info_log(vertexShader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteProgram(program);
    }
    return program;
}


GLuint setupTexture_Any(void* data, int x, int y, GLenum internalFormat, GLint format, GLenum type) {
    GLuint id;
    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, format, type, data);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    return id;
}

GLuint setupTexture(void* data, int x, int y) {
    return setupTexture_Any(data, x, y, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
}

void setParam(GLsizei program, UniformData data) {
    GLint loc = glGetUniformLocation(program, (char*)data.name);
    if((data.data == NULL && !(data.dataStructure==Texture)) || loc==-1) return;
    switch(data.dataStructure){
        case One:
            switch(data.dataType){
                case GL_FLOAT: glUniform1f(loc, *(GLfloat*)data.data); break;
                case GL_INT: glUniform1i(loc, *(GLint*)data.data); break;
            } break;
        case Vec2:
            switch(data.dataType){
                case GL_FLOAT: glUniform2fv(loc, 1, (GLfloat*)data.data); break;
                case GL_INT: glUniform2iv(loc, 1, (GLint*)data.data); break;
            } break;
        case Vec3:
            switch(data.dataType){
                case GL_FLOAT: glUniform3fv(loc, 1, (GLfloat*)data.data); break;
                case GL_INT: glUniform3iv(loc, 1, (GLint*)data.data); break;
            } break;
        case Vec4:
            switch(data.dataType){
                case GL_FLOAT: glUniform4fv(loc, 1, (GLfloat*)data.data); break;
                case GL_INT: glUniform4iv(loc, 1, (GLint*)data.data); break;
            } break;
        case M2:
            switch(data.dataType){
                case GL_FLOAT: glUniformMatrix2fv(loc, 1, false, (GLfloat*)data.data); break;
            } break;
        case M3:
            switch(data.dataType){
                case GL_FLOAT: glUniformMatrix3fv(loc, 1, false, (GLfloat*)data.data); break;
            } break;
        case M4:
            switch(data.dataType){
                case GL_FLOAT: glUniformMatrix4fv(loc, 1, false, (GLfloat*)data.data); break;
            } break;
        case Texture:
            ;
            glActiveTexture(GL_TEXTURE0+data.texnum);
            glBindTexture(GL_TEXTURE_2D, data.id);
            glUniform1i(loc, data.texnum);
            break;
    }
}
#if 0
void load_wavefront_obj(String path, u32 arrayBuffer) {
    FILE* file = fopen(path, "rb");

    float* vertices = malloc(1<<10); 
    float* uvs = malloc(1<<9); 
    float* normals = malloc(1<<10); 
    float* data = malloc(1<<13);

    int remaining = 1;
    int vertI = 0;
    int normI = 0;
    int uvI = 0;
    int elI = 0;
    while(remaining) {
        char header[128];
        remaining = fscanf(file, "%s", header); 
        if(remaining == EOF) break;
        if(header[0] == '#') continue; 
        else if(strcmp(header, "v") == 0) {
            float verts[3];
            fscanf(file, "%f %f %f\n", verts, verts+1, verts+2);
            vertices[vertI++] = vertices[0];
            vertices[vertI++] = vertices[1];
            vertices[vertI++] = vertices[2];
        }
        else if(strcmp(header, "vn") == 0) {
            float verts[3];
            fscanf(file, "%f %f %f\n", verts, verts+1, verts+2);
            normals[normI++] = verts[0];
            normals[normI++] = verts[1];
            normals[normI++] = verts[2];
        }
        else if( strcmp(header, "vt") == 0) {
            float verts[2];
            fscanf(file, "%f %f\n", verts, verts+1);
            uvs[uvI++] = verts[0];
            uvs[uvI++] = verts[1];
        }
        else if( strcmp(header, "f") == 0) {
            unsigned int verts[3];
            unsigned int norms[3];
            unsigned int uvss[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", verts, uvss, norms, verts+1, uvss+1, norms+1, verts+2, uvss+2, norms+2);
            if(matches != 9){
                printf("Format for face not valid for this parser. Format must be v/vt/vn v/vt/vn v/vt/vn\n");
                return;
            }
            data[elI++] = vertices[verts[0]-1];
            data[elI++] = uvs[uvss[0]-1];
            data[elI++] = normals[norms[0]-1];

            data[elI++] = vertices[verts[1]-1];
            data[elI++] = vertices[uvss[1]-1];
            data[elI++] = normals[norms[1]-1];

            data[elI++] = vertices[verts[2]-1];
            data[elI++] = uvs[uvss[2]-1];
            data[elI++] = normals[norms[2]-1];
        }
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, elI*sizeof(float), data, GL_STATIC_DRAW);

    fclose(file);

}
#endif

#endif
