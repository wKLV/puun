#include <math.h>
#include <GL/glew.h>
#include <SDL/SDL.h>

#include <puun/puun.h>
#include <puun/types.h>
#include <puun/gf/mesh.h>

struct Game_Memory {
    b32 isRunning;
    b32 isKeyPressed;
    u8 program;
    struct gf_Mesh mesh;
    struct gf_Mesh mesh2;
    struct gf_Mesh mesh3;
    v3 vertices[1<<13];
    v3 normals[1<<13];
    v2 uvs[1<<13];
    m4 worldMatrix;
    v3 rotation;
};


void init(Data game_memory) {
    struct Game_Memory* mem = (struct Game_Memory*)game_memory;
    
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(1);

    u8 vs[] = "attribute vec3 position;\n\
               attribute vec3 normals;\n\
               attribute vec2 uv;\n\
               uniform mat4 world;\n\
               varying vec2 uvs;\n\
               varying vec3 normal;\n\
                 void main(){\n\
                     gl_Position=world*vec4(position, 1);\n\
                     uvs = uv; normal = (world*vec4(normals,0)).xyz;\n\
                 }";
    u8 fs[] = "precision mediump float; \n\
               varying float num;\n\
               varying vec2 uvs;\n\
               varying vec3 normal;\n\
                 void main() {\n\
                    vec4 colour = vec4(0.5, 0.75, 0.25, 1.0);\n\
                    vec4 specular = vec4(0.77, 0.9, 0.5, 1.0);\n\
                 //   colour = vec4(uvs, 0, 0);\n\
                    vec3 normals = normalize(normal);\n\
                    vec3 light = normalize(vec3(1, -.2, 0));\n\
                    float N = dot(light,normals);\n\
                    gl_FragColor = mix(colour, specular, N);\n\
                 }";

    mem->program = setupProgram(vs, 0, fs, 0);
    mem->mesh = init_mesh();
    mem->mesh.vertices = mem->vertices;
    mem->mesh.uvs = mem->uvs;
    mem->mesh.normals = mem->normals;

    //LET'S DO A CUBE!!
#if 0
    i32 i = 0;
    {
        mem->normals[i] = new_v3(0,0,-1);
        mem->uvs[i] = new_v2(0,0);
        mem->vertices[i] = new_v3(-1,-1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,0,-1);
        mem->uvs[i] = new_v2(1,0);
        mem->vertices[i] = new_v3(+1,-1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,0,-1);
        mem->uvs[i] = new_v2(0,1);
        mem->vertices[i] = new_v3(-1,+1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,0,-1);
        mem->uvs[i] = new_v2(1,0);
        mem->vertices[i] = new_v3(+1,-1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,0,-1);
        mem->uvs[i] = new_v2(0,1);
        mem->vertices[i] = new_v3(-1,+1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,0,-1);
        mem->uvs[i] = new_v2(1,1);
        mem->vertices[i] = new_v3(+1,+1,-1);
        mem->elements[i] = i;
        i++;
    }
    {
        mem->normals[i] = new_v3(0,0,+1);
        mem->uvs[i] = new_v2(0,0);
        mem->vertices[i] = new_v3(-1,-1,+1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,0,+1);
        mem->uvs[i] = new_v2(1,0);
        mem->vertices[i] = new_v3(+1,-1,+1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,0,+1);
        mem->uvs[i] = new_v2(0,1);
        mem->vertices[i] = new_v3(-1,+1,+1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,0,+1);
        mem->uvs[i] = new_v2(1,0);
        mem->vertices[i] = new_v3(+1,-1,+1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,0,+1);
        mem->uvs[i] = new_v2(0,1);
        mem->vertices[i] = new_v3(-1,+1,+1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,0,+1);
        mem->uvs[i] = new_v2(1,1);
        mem->vertices[i] = new_v3(+1,+1,+1);
        mem->elements[i] = i;
        i++;
    }
    {
        mem->normals[i] = new_v3(0,-1,0);
        mem->uvs[i] = new_v2(0,0);
        mem->vertices[i] = new_v3(-1,-1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,-1,0);
        mem->uvs[i] = new_v2(1,0);
        mem->vertices[i] = new_v3(+1,-1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,-1,0);
        mem->uvs[i] = new_v2(0,1);
        mem->vertices[i] = new_v3(-1,-1,+1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,-1,0);
        mem->uvs[i] = new_v2(1,0);
        mem->vertices[i] = new_v3(+1,-1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,-1,0);
        mem->uvs[i] = new_v2(0,1);
        mem->vertices[i] = new_v3(-1,-1,+1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,-1,0);
        mem->uvs[i] = new_v2(1,1);
        mem->vertices[i] = new_v3(+1,-1,+1);
        mem->elements[i] = i;
        i++;
    }
    {
        mem->normals[i] = new_v3(0,+1,0);
        mem->uvs[i] = new_v2(0,0);
        mem->vertices[i] = new_v3(-1,+1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,+1,0);
        mem->uvs[i] = new_v2(1,0);
        mem->vertices[i] = new_v3(+1,+1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,+1,0);
        mem->uvs[i] = new_v2(0,1);
        mem->vertices[i] = new_v3(-1,+1,+1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,+1,0);
        mem->uvs[i] = new_v2(1,0);
        mem->vertices[i] = new_v3(+1,+1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,+1,0);
        mem->uvs[i] = new_v2(0,1);
        mem->vertices[i] = new_v3(-1,+1,+1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(0,+1,0);
        mem->uvs[i] = new_v2(1,1);
        mem->vertices[i] = new_v3(+1,+1,+1);
        mem->elements[i] = i;
        i++;
    }
    {
        mem->normals[i] = new_v3(-1,0,0);
        mem->uvs[i] = new_v2(0,0);
        mem->vertices[i] = new_v3(-1,-1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(-1,0,0);
        mem->uvs[i] = new_v2(1,0);
        mem->vertices[i] = new_v3(-1,+1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(-1,0,0);
        mem->uvs[i] = new_v2(0,1);
        mem->vertices[i] = new_v3(-1,-1,+1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(-1,0,0);
        mem->uvs[i] = new_v2(1,0);
        mem->vertices[i] = new_v3(-1,+1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(-1,0,0);
        mem->uvs[i] = new_v2(0,1);
        mem->vertices[i] = new_v3(-1,-1,+1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(-1,0,0);
        mem->uvs[i] = new_v2(1,1);
        mem->vertices[i] = new_v3(-1,+1,+1);
        mem->elements[i] = i;
        i++;
    }
    {
        mem->normals[i] = new_v3(+1,0,0);
        mem->uvs[i] = new_v2(0,0);
        mem->vertices[i] = new_v3(+1,-1,-1);
        mem->elements[i] = i;
        i++;
        mem->normals[i] = new_v3(+1,0,0);
        mem->uvs[i] = new_v2(1,0);
        mem->vertices[i] = new_v3(+1,+1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(+1,0,0);
        mem->uvs[i] = new_v2(0,1);
        mem->vertices[i] = new_v3(+1,-1,+1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(+1,0,0);
        mem->uvs[i] = new_v2(1,0);
        mem->vertices[i] = new_v3(+1,+1,-1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(+1,0,0);
        mem->uvs[i] = new_v2(0,1);
        mem->vertices[i] = new_v3(+1,-1,+1);
        mem->elements[i] = i;
        i++;

        mem->normals[i] = new_v3(+1,0,0);
        mem->uvs[i] = new_v2(1,1);
        mem->vertices[i] = new_v3(+1,+1,+1);
        mem->elements[i] = i;
        i++;
    }

    mem->mesh.vertices_length = 36;
    mem->mesh.uvs_length = 36;
    mem->mesh.normals_length = 36;
    mem->mesh.triangles_length = 36;
    
#else
    load_obj_from_file(&mem->mesh, "../assets/cool.obj");
#endif
    prepare_mesh(mem->mesh);
/*
    mem->mesh2 = init_mesh(); 
    mem->mesh2.vertices = mem->vertices;
    mem->mesh2.uvs = mem->uvs;
    mem->mesh2.normals = mem->normals;
    load_obj_from_file(&mem->mesh2, "../assets/Lugae.obj");
    prepare_mesh(mem->mesh2);
*/
    mem->worldMatrix = identity_m4();
}

void updateNrender(Data game_memory) {
    struct Game_Memory* mem = (struct Game_Memory*)game_memory;

    puun_KEY character;
    getKeyboardKey(&character);
    if((!mem->isKeyPressed) && character.isPressed) {
        mem->isKeyPressed = true;
        bool isF4 = (character.key == 29) && (character.alt & puun_ALT);
        if(isF4) {
            game_die(game_memory);
            return;
        }
        bool isF5 = (character.key == 30);
        if(isF5) {
            mem->worldMatrix = append_m4(mem->worldMatrix, scale_m4(1.0/0.8));
        }
        bool isSpace = (character.key == SDLK_SPACE);
        if(isSpace) {
            mem->worldMatrix = append_m4(mem->worldMatrix, scale_m4(0.8));
        }
        bool isUp = (character.key == 'w');
        if(isUp) {
            mem->worldMatrix = append_m4(mem->worldMatrix, traslate_m4(new_v3(0,0.2,0)));
        }
        bool isDown = (character.key == 's');
        if(isDown) {
            mem->worldMatrix = append_m4(mem->worldMatrix, traslate_m4(new_v3(0,-0.2,0)));
        }
        bool isBottom = (character.key == 'q');
        if(isBottom) {
            mem->worldMatrix = append_m4(mem->worldMatrix, traslate_m4(new_v3(0,0,+0.2)));
        }
        bool isTop = (character.key == 'e');
        if(isTop) {
            mem->worldMatrix = append_m4(mem->worldMatrix, traslate_m4(new_v3(0,0,-0.2)));
        }
        bool isLeft = (character.key == 'a');
        if(isLeft) {
            mem->worldMatrix = append_m4(mem->worldMatrix, traslate_m4(new_v3(-0.2,0,0)));
        }
        bool isRight = (character.key == 'd');
        if(isRight) {
            mem->worldMatrix = append_m4(mem->worldMatrix, traslate_m4(new_v3(0.2,0,0)));
        }
        bool isX = (character.key == 'x');
        if(isX) {
            mem->worldMatrix = append_m4(mem->worldMatrix, rot_axis_angle_m4(new_v3(1,0,0), TAU/160));
        }
        bool isZ = (character.key == 'z');
        if(isZ) {
            mem->worldMatrix = append_m4(mem->worldMatrix, rot_axis_angle_m4(new_v3(0,0,1), TAU/160));
        }
        bool isY = (character.key == 'y');
        if(isY) {
            mem->worldMatrix = append_m4(mem->worldMatrix, rot_axis_angle_m4(new_v3(0,1,0), TAU/160));
        }
    }
    if(!character.isPressed) mem->isKeyPressed = false;


    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if 0
    UniformData uniforms[3] = ZERO_STRUCT;
    UniformData uni1 = uniforms[0];
    uni1 = TODO;
    
    render_meshList(&mem->mesh, 1, uniforms, 3);
#endif

    glUniformMatrix4fv(glGetUniformLocation(mem->program, "world"), 1, false, (f32*)&mem->worldMatrix);
    render_meshes(&mem->mesh, 1, mem->program);
    //render_meshes(&mem->mesh2, 1, mem->program);
    puun_SWAP_BUFFERS();
}

void game_die(Data game_memory) {
   platform_die(); 
}
