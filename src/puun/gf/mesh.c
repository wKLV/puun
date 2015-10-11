#include "mesh.h"

struct gf_Mesh init_mesh() {
    struct gf_Mesh result = ZERO_STRUCT;

    glGenBuffers(1, &result.vertsBuffer);
    glGenBuffers(1, &result.trisBuffer);

    return result;
}

void updateBuffer(Data info, u32 buffer);

void prepare_mesh(struct gf_Mesh mesh) {
    u32 stride  = 3*sizeof(f32);
    stride += 2*sizeof(f32);
    stride += 3*sizeof(f32);
    f32* data = malloc(mesh.vertices_length*stride);
    i32 i, dataI;
    for(i=0, dataI=0; i<mesh.vertices_length; ++i) {
        i32 j;
        for(j=0; j<3; ++j)
            data[dataI++] = mesh.vertices[i].els[j];
        for(j=0; j<2; ++j)
            data[dataI++] = mesh.uvs[i].els[j];
        for(j=0; j<3; ++j)
            data[dataI++] = mesh.normals[i].els[j];
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vertsBuffer);

    glBufferData(GL_ARRAY_BUFFER, 
            mesh.vertices_length*stride,
            data, GL_STATIC_DRAW);

   // NOTE: Prepare triangle face buffer 
    stride = 3*sizeof(u32);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.trisBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.triangles_length*stride,
            mesh.triangles, GL_STATIC_DRAW);
}

void render_meshes(struct gf_Mesh* meshes, i32 manyMeshes, u8 program) {
    glUseProgram(program);

    i32 m, i;
    for(m=0; m<manyMeshes; ++m) {
        struct gf_Mesh mesh = meshes[m];

        for(i=0; i<mesh.uniforms_length; ++i) {
            setParam(program, mesh.uniforms[i]);
        }

        GLint pos = glGetAttribLocation(program, "position");
        GLint uv = glGetAttribLocation(program, "uv");
        GLint norms = glGetAttribLocation(program, "normals");
        u32 stride  = 3*sizeof(f32);
        stride += 2*sizeof(f32);
        stride += 3*sizeof(f32);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.vertsBuffer);
        glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE,
                stride, 0);
        glEnableVertexAttribArray(pos);

        glVertexAttribPointer(uv, 3, GL_FLOAT, GL_FALSE,
                stride, (Data) (3*sizeof(f32)));
        glEnableVertexAttribArray(uv);

        glVertexAttribPointer(norms, 3, GL_FLOAT, GL_FALSE,
                stride, (Data)((3+2)*sizeof(f32)));
        glEnableVertexAttribArray(norms);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.trisBuffer);

        glDrawElements(GL_TRIANGLES, mesh.triangles_length*3, GL_UNSIGNED_INT, (void*)0);
    }
    
}
struct gf_Mesh load_obj_from_file(String path) {
    struct gf_Mesh result = ZERO_STRUCT;
    return result;    
}
