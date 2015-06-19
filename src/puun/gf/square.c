#include "gl_help.c"
#include "square.h"

static GLuint vertexBuffer,
       elementBuffer,
       positionLoc;

void square_init() {
    GLfloat vertex[] =
    {
        -1.0, -1.0,
        1.0, -1.0,
        -1.0, 1.0,
        1.0, 1.0
    };

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    GLushort elements[] = {0,1,2,3};
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

}

GLuint square_createType(char* vertexSource, int vertexSourceSize, char* fragmentSource, int fragmentSourceSize) {
    char* vs = vertexSource;
    char* fs = fragmentSource;
    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLint shader_ok;

    glShaderSource(vertexShader, 1, (const GLchar**)&vs, 0);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shader_ok);
    if(shader_ok == GL_FALSE) {
        GLsizei length = 1000;
        printf("Failed to compile vertex shader\n", 0);
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

    positionLoc = glGetAttribLocation(program, "position");

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
        positionLoc,       /* attribute */
        2,                                /* size */
        GL_FLOAT,                         /* type */
        GL_FALSE,                         /* normalized? */
        sizeof(GLfloat)*2,                /* stride */
        (void*)0                          /* array buffer offset */
    );
    glEnableVertexAttribArray(positionLoc);

    return program;

}
void square_render(u8 program, GLsizei elems, UniformData* unis, int unis_length) {
    glUseProgram(program);
    int i;
    for(i=0; i<unis_length; i++){
        setParam(program, unis[i]);
    }

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0);
}
