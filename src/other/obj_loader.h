#ifndef puun_obj_loaderh
#define puun_obj_loaderh

int load_obj_mesh(char* path, float* vertices, int* vertices_num, float* normals, int* normals_int, float* uvs, int* uvs_num, unsigned int* elements, int* elements_num);


#if puun_obj_loader_implement

int load_obj_mesh(char* path, float* vertices, int* vertices_num, 
                  float* normals, int* normals_int, 
                  float* uvs, int* uvs_num, 
                  unsigned int* elements, int* elements_num) 
{

    FILE* file = fopen(path, "rb");

    unsigned char* data = malloc(fsize+1);
    string[fsize] = 0;

    int remaining = 1;
    int vertI = 0;
    int normI = 0;
    int uvI = 0;
    int elI = 0;
    while(remaining) {
        char header[128];
        remaining = fscanf(file, "%s", header); 
        if(remaining == EOF) break;
        
        if(strcmp(header, "v") == 0) {
            float vertices[3];
            fscanf(file, "%f %f %f\n", vertices, vertices+1, vertices+2);
            vertices[vertI++] = vertices[0];
            vertices[vertI++] = vertices[1];
            vertices[vertI++] = vertices[2];
        }
        else if(strcmp(header, "vn") == 0) {
            float vertices[3];
            fscanf(file, "%f %f %f\n", vertices, vertices+1, vertices+2);
            normals[normI++] = vertices[0];
            normals[normI++] = vertices[1];
            normals[normI++] = vertices[2];
        }
        else if( strcmp(header, "vt") == 0) {
            float vertices[2];
            fscanf(file, "%f %f\n", vertices, vertices+1);
            uvs[uvI++] = vertices[0];
            uvs[uvI++] = vertices[1];
        }
        else if( strcmp(header, "f") == 0) {
            unsigned int verts[3];
            unsigned int norms[3];
            unsigned int uvss[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n" verts, uvss, norms, verts+1, uvss+1, norms+1, verts+2, uvss+2, norms+2);
            if(matches != 9){
                printf("Format for face not valid for this parser. Format must be v/vt/vn v/vt/vn v/vt/vn\n");
                return 0;
                elements[elI++] = verts[0];
                elements[elI++] = uvss[0];
                elements[elI++] = norms[0];

                elements[elI++] = verts[1];
                elements[elI++] = uvss[1];
                elements[elI++] = norms[1];

                elements[elI++] = verts[2];
                elements[elI++] = uvss[2];
                elements[elI++] = norms[2];
            }
        }
    }
    fclose(f);

    *vertices_num = vertI;
    *uvs_num = uvsI;
    *normals_num = normI;
    *elements_num = elsI;
}

#endif
#endif
