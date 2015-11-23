#include "mesh.h"
#include <string.h>

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
    s32 i, dataI;
    for(i=0, dataI=0; i<mesh.vertices_length; ++i) {
        s32 j;
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

    free(data);
}

void render_meshes(struct gf_Mesh* meshes, s32 manyMeshes, u8 program) {
    glUseProgram(program);

    s32 m, i;
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

        glDrawArrays(GL_TRIANGLES, 0, mesh.vertices_length);
    }
    
}
void load_wavefront_from_file(struct gf_Mesh* mesh, String path) {
    struct gf_Mesh result = *mesh;
    
    FILE* file = fopen(path, "rb");

    v3* vertices = malloc(1<<16); 
    v2* uvs = malloc(1<<16); 
    v3* normals = malloc(1<<16); 

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
            vertices[vertI++] = new_v3(verts[0], verts[1], verts[2]);
        }
        else if(strcmp(header, "vn") == 0) {
            float verts[3];
            fscanf(file, "%f %f %f\n", verts, verts+1, verts+2);
            normals[normI++] = new_v3(verts[0], verts[1], verts[2]);
        }
        else if( strcmp(header, "vt") == 0) {
            float verts[2];
            fscanf(file, "%f %f\n", verts, verts+1);
            uvs[uvI++] = new_v2(verts[0], verts[1]);
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
            result.vertices[elI] = vertices[verts[0]-1];
            result.normals[elI] = normals[norms[0]-1];
            result.uvs[elI] = uvs[uvss[0]-1];
            elI++;
            
            result.vertices[elI] = vertices[verts[1]-1];
            result.normals[elI] = normals[norms[1]-1];
            result.uvs[elI] = uvs[uvss[1]-1];
            elI++;
            
            result.vertices[elI] = vertices[verts[2]-1];
            result.normals[elI] = normals[norms[2]-1];
            result.uvs[elI] = uvs[uvss[2]-1];
            elI++;
        }
    }
    fclose(file);

    free(vertices);
    free(uvs);
    free(normals);

    result.vertices_length = elI;

    *mesh = result;
}
