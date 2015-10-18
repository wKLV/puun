#include "gl_help.h"
#include "../math_types.h"

struct gf_Mesh {
    DEF_ARR(vertices, v3);
    DEF_ARR(uvs, v2);
    DEF_ARR(normals, v3);
    DEF_ARR(triangles, u32);
    u32 trisBuffer;
    u32 vertsBuffer;
    DEF_ARR(uniforms, UniformData);
    u8 vao;
};

void load_obj_from_file(struct gf_Mesh* mesh, String path);

struct gf_Mesh init_mesh();
void prepare_mesh(struct gf_Mesh mesh);
void render_meshes(struct gf_Mesh* meshes, i32 manyMeshes, u8 program);
