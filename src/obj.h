#include <tice.h>
# include "mathematics.h"

#define ASPECT_RATIO 1.33333
#define MAX_VERTICES 64
#define MAX_TRIANGLES (MAX_VERTICES * 2)

struct obj3
{
    int vertices_number;
    struct vec3 vertices[MAX_VERTICES];
    int triangle_number;
    struct vec3 triangles[MAX_TRIANGLES]; // vec3 {x, y, z} with x, y and z the index of vertices that can form a triangle
    struct vec3 pos;
    struct uint_vec3 rot;
};
struct cam
{
    fixed_point fov;
    fixed_point aspectRatio;
    fixed_point zNear;
    fixed_point zFar;
    struct vec3 pos;
};

void debug_obj3_vertices(struct obj3 *Obj, char Name[])
{
    dbg_printf("vertices numbers = %d", Obj->vertices_number);
    for (int i = 0; i < Obj->vertices_number; i++){
        dbg_printf("\ni = %d", i);
        debug_vec3(Obj->vertices[i], Name);
    }
}