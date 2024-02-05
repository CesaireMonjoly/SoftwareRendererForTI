#include <tice.h>
//#include <math.h>

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

/** Generated using Dr LUT - Free Lookup Table Generator
  * https://github.com/ppelikan/drlut
  **/
const uint8_t SIN_LUT[91] = {
        0,  4,  9, 13, 18, 22, 27, 31, 35, 40, 44, 49, 53,
        57, 62, 66, 70, 75, 79, 83, 87, 91, 96,100,104,108,
        112,116,120,124,127,131,135,139,143,146,150,153,157,
        160,164,167,171,174,177,180,183,186,190,192,195,198,
        201,204,206,209,211,214,216,219,221,223,225,227,229,
        231,233,235,236,238,240,241,243,244,245,246,247,248,
        249,250,251,252,253,253,254,254,254,255,255,255,255 };
//F*ck Taylor's Series, Look Up Table go brrr


void debug_vec2(struct vec2 Obj, char Name[])
{
    dbg_printf("\n%s.x = %d\n", Name, Obj.x);
    dbg_printf("%s.y = %d\n", Name, Obj.y);
}

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