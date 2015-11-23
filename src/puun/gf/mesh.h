#include "gl_help.h"
#include "../math_types.h"

typedef struct gf_Mesh {
    v3* vertices;
    v2* uvs;
    v3* normals;
    u32* triangles;
    u32 vertices_length;
    u32 trisBuffer;
    u32 vertsBuffer;
    DEF_ARR(uniforms, UniformData);
    u8 vao;
} gf_Mesh;

void load_wavefront_from_file(struct gf_Mesh* mesh, String path);

struct gf_Mesh init_mesh();
void prepare_mesh(struct gf_Mesh mesh);
void render_meshes(struct gf_Mesh* meshes, s32 manyMeshes, u8 program);
