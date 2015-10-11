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
    v3 vertices[36];
    v3 normals[36];
    v2 uvs[36];
    u32 elements[36];
    v4 worldMatrix[4];
    v3 rotation;
};

void identity(v4 matrix[4]) {
    i32 i=0, j=0;
    for(i=0; i<4; ++i) for(j=0; j<4; ++j) {
        if(i==j) (matrix)[i].els[j] = 1.f;
        else (matrix)[i].els[j] = 0.f;
    }
}
void scale(v4 matrix[4], f32 scalar) {
    i32 i=0, j=0;
    for(i=0; i<3; ++i) for(j=0; j<3; ++j) {
        (matrix)[i].els[j] *= scalar;
    }
}
void rotx(v4 matrix[4], f32 angle) {
    f32 co = cos(angle), si = sin(angle);
    matrix[0].els[0] = 1;
    matrix[0].els[1] = 0;
    matrix[0].els[2] = 0;

    matrix[1].els[0] = 0;
    matrix[1].els[1] = co;
    matrix[1].els[2] = si;

    matrix[2].els[0] = 0;
    matrix[2].els[1] = -si;
    matrix[2].els[2] = co;
}

void rotz(v4 matrix[4], f32 angle) {
    f32 co = cos(angle), si = sin(angle);
    matrix[0].els[0] = co;
    matrix[0].els[1] = si;
    matrix[0].els[2] = 0;

    matrix[1].els[0] = -si;
    matrix[1].els[1] = co;
    matrix[1].els[2] = 0;

    matrix[2].els[0] = 0;
    matrix[2].els[1] = 0;
    matrix[2].els[2] = 1;
}
void roty(v4 matrix[4], f32 angle) {
    f32 co = cos(angle), si = sin(angle);
    matrix[0].els[0] = co;
    matrix[0].els[1] = 0;
    matrix[0].els[2] = -si;

    matrix[1].els[0] = 0;
    matrix[1].els[1] = 1;
    matrix[1].els[2] = 0;

    matrix[2].els[0] = si;
    matrix[2].els[1] = 0;
    matrix[2].els[2] = co;
}
void traslate(v4 matrix[4], v3 traslate) {
    matrix[3] = add_v4(new_v4(traslate.x, traslate.y, traslate.z, 0), matrix[3]);
}


void init(Data game_memory) {
    struct Game_Memory* mem = (struct Game_Memory*)game_memory;
    struct gf_Mesh mesh = init_mesh();
    
    u8 vs[] = "attribute vec3 position;\n\
               attribute vec3 normals;\n\
               attribute vec2 uv;\n\
               uniform mat4 world;\n\
               varying vec2 uvs;\n\
               varying vec3 normal;\n\
                 void main(){\n\
                     gl_Position=world*vec4(position, 1);\n\
                     uvs = uv; normal = world*vec4(normals,0);\n\
                 }";
    u8 fs[] = "precision mediump float; \n\
               varying float num;\n\
               varying vec2 uvs;\n\
               varying vec3 normal;\n\
                 void main() {\n\
                    vec4 colour = vec4(0.5, 0.75, 0.25, 1.0);\n\
                    vec4 specular = vec4(0.77, 0.9, 0.5, 1.0);\n\
                    colour = vec4(uvs, 0, 0);\n\
                    vec3 normals = normalize(normal);\n\
                    vec3 light = normalize(vec3(1, 0, -1));\n\
                    float N = dot(light,normals);\n\
                    gl_FragColor = mix(colour, specular, N);\n\
                 }";

    mem->program = setupProgram(vs, 0, fs, 0);

    //LET'S DO A CUBE!!

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

        mem->elements[i] = i-2;
        i++;

        mem->elements[i] = i-2;
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

        mem->elements[i] = i-2;
        i++;

        mem->elements[i] = i-2;
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

        mem->elements[i] = i-2;
        i++;

        mem->elements[i] = i-2;
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

        mem->elements[i] = i-2;
        i++;

        mem->elements[i] = i-2;
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

        mem->elements[i] = i-2;
        i++;

        mem->elements[i] = i-2;
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

        mem->elements[i] = i-2;
        i++;

        mem->elements[i] = i-2;
        i++;

        mem->normals[i] = new_v3(+1,0,0);
        mem->uvs[i] = new_v2(1,1);
        mem->vertices[i] = new_v3(+1,+1,+1);
        mem->elements[i] = i;
        i++;
    }

    mesh.vertices = mem->vertices;
    mesh.vertices_length = 36;
    mesh.uvs = mem->uvs;
    mesh.uvs_length = 36;
    mesh.normals = mem->normals;
    mesh.normals_length = 36;
    mesh.triangles = mem->elements;
    mesh.triangles_length = 36;
    
    mem->mesh = mesh;
    prepare_mesh(mem->mesh);
    identity(mem->worldMatrix);
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
            scale(mem->worldMatrix, 1.0/0.8);
            return;
        }
        bool isSpace = (character.key == SDLK_SPACE);
        if(isSpace) {
            scale(mem->worldMatrix, 0.8);
           return;
        }
        bool isUp = (character.key == 'w');
        if(isUp) {
            traslate(mem->worldMatrix, new_v3(0,0.2,0));
           return;
        }
        bool isDown = (character.key == 's');
        if(isDown) {
            traslate(mem->worldMatrix, new_v3(0,-0.2,0));
           return;
        }
        bool isLeft = (character.key == 'a');
        if(isLeft) {
            traslate(mem->worldMatrix, new_v3(-0.2,0,0));
           return;
        }
        bool isRight = (character.key == 'd');
        if(isRight) {
            traslate(mem->worldMatrix, new_v3(0.2,0,0));
           return;
        }
        bool isX = (character.key == 'x');
        if(isX) {
            mem->rotation.x = fmod(mem->rotation.x + (TAU/8.f), TAU);
            rotx(mem->worldMatrix, mem->rotation.x);
           return;
        }
        bool isZ = (character.key == 'z');
        if(isZ) {
            mem->rotation.z = fmod(mem->rotation.z + (TAU/8.f), TAU);
            rotz(mem->worldMatrix, mem->rotation.z);
           return;
        }
        bool isY = (character.key == 'y');
        if(isY) {
            mem->rotation.y = fmod(mem->rotation.y + (TAU/8.f), TAU);
            roty(mem->worldMatrix, mem->rotation.y);
           return;
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

    glUniformMatrix4fv(glGetUniformLocation(mem->program, "world"), 1, false, (f32*)mem->worldMatrix);
    render_meshes(&mem->mesh, 1, mem->program);
    puun_SWAP_BUFFERS();
}

void game_die(Data game_memory) {
   platform_die(); 
}
