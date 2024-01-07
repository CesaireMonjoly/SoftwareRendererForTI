#include <tice.h>
#include <math.h>

#define ASPECT_RATIO 1.333333
#define MAX_VERTICES 64
#define MAX_TRIANGLES (MAX_VERTICES * 2)

struct vec3f
{
    float x;
    float y;
    float z;
};

struct vec3
{
    int x;
    int y;
    int z;
};
struct vec2
{
    int x;
    int y;
};
struct obj3
{
    int vertices_number;
    struct vec3 vertices[MAX_VERTICES]; // oj3->vertices[0] refer to the "center"/"position" of the object but the others vertices are not related to it
    int triangle_number;
    struct vec3 triangles[MAX_TRIANGLES]; // ve3 {x, y, z} with x, y and z (all different from 0) the index of vertices that can form a triangle
    struct vec3 pos;
};
struct cam
{
    float fov;
    float aspectRatio;
    int zNear;
    int zFar;
    struct vec3 pos;
};

void debug_vec3(struct vec3 Obj, char Name[])
{
    dbg_printf("\n%s.x = %d\n", Name, Obj.x);
    dbg_printf("%s.y = %d\n", Name, Obj.y);
    dbg_printf("%s.z = %d\n", Name, Obj.z);
}
void debug_vec3f(struct vec3f Obj, char Name[])
{
    dbg_printf("\n%s.x = %f\n", Name, Obj.x);
    dbg_printf("%s.y = %f\n", Name, Obj.y);
    dbg_printf("%s.z = %f\n", Name, Obj.z);
}
void debug_obj3_vertices(struct obj3 *Obj, char Name[])
{
    dbg_printf("vertices numbers = %d", Obj->vertices_number);
    for (int i = 0; i < Obj->vertices_number; i++){
        dbg_printf("\ni = %d", i);
        debug_vec3(Obj->vertices[i], Name);
    }
}