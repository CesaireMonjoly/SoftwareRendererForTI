#include "fixed.h"

#define ASPECT_RATIO 1.33333
#define MAX_VERTICES 64
#define MAX_TRIANGLES (MAX_VERTICES * 2)


struct mat4
{
    fixed_point m[4][4];
};
struct vec4
{
    fixed_point x;
    fixed_point y;
    fixed_point z;
    fixed_point w;
};
struct vec3
{
    fixed_point x;
    fixed_point y;
    fixed_point z;
};
struct vec2
{
    fixed_point x;
    fixed_point y;
};


const int24_t SIN_LUT[361] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10,
                              11, 12, 13, 14, 15, 17, 18, 19, 20, 21,
                              22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
                              32, 33, 34, 35, 36, 37, 38, 39, 39, 40,
                              41, 42, 43, 44, 44, 45, 46, 47, 48, 48,
                              49, 50, 50, 51, 52, 52, 53, 54, 54, 55,
                              55, 56, 57, 57, 58, 58, 58, 59, 59, 60,
                              60, 61, 61, 61, 62, 62, 62, 62, 63, 63,
                              63, 63, 63, 64, 64, 64, 64, 64, 64, 64,
                              64, 64, 64, 64, 64, 64, 64, 64, 63, 63,
                              63, 63, 63, 62, 62, 62, 62, 61, 61, 61,
                              60, 60, 59, 59, 58, 58, 58, 57, 57, 56,
                              55, 55, 54, 54, 53, 52, 52, 51, 50, 50,
                              49, 48, 48, 47, 46, 45, 44, 44, 43, 42,
                              41, 40, 39, 39, 38, 37, 36, 35, 34, 33,
                              32, 31, 30, 29, 28, 27, 26, 25, 24, 23,
                              22, 21, 20, 19, 18, 17, 15, 14, 13, 12,
                              11, 10, 9, 8, 7, 6, 4, 3, 2, 1,
                              0, -1, -2, -3, -4, -6, -7, -8, -9, -10,
                              -11, -12, -13, -14, -15, -17, -18, -19, -20, -21,
                              -22, -23, -24, -25, -26, -27, -28, -29, -30, -31,
                              -32, -33, -34, -35, -36, -37, -38, -39, -39, -40,
                              -41, -42, -43, -44, -44, -45, -46, -47, -48, -48,
                              -49, -50, -50, -51, -52, -52, -53, -54, -54, -55,
                              -55, -56, -57, -57, -58, -58, -58, -59, -59, -60,
                              -60, -61, -61, -61, -62, -62, -62, -62, -63, -63,
                              -63, -63, -63, -64, -64, -64, -64, -64, -64, -64,
                              -64, -64, -64, -64, -64, -64, -64, -64, -63, -63,
                              -63, -63, -63, -62, -62, -62, -62, -61, -61, -61,
                              -60, -60, -59, -59, -58, -58, -58, -57, -57, -56,
                              -55, -55, -54, -54, -53, -52, -52, -51, -50, -50,
                              -49, -48, -48, -47, -46, -45, -44, -44, -43, -42,
                              -41, -40, -39, -39, -38, -37, -36, -35, -34, -33,
                              -32, -31, -30, -29, -28, -27, -26, -25, -24, -23,
                              -22, -21, -20, -19, -18, -17, -15, -14, -13, -12,
                              -11, -10, -9, -8, -7, -6, -4, -3, -2, -1,
                              0};

int sine(int angle)
{
    return SIN_LUT[angle];
}

int cosine(int angle)
{
    return SIN_LUT[90-angle];
}

/*
void debug_vec2(struct vec2 Vec, char Name[])
{
    dbg_printf("\n%s.x = %f\n", Name, fixed_to_double(Vec.x));
    dbg_printf("%s.y = %f\n", Name, fixed_to_double(Vec.y));
}

void debug_vec3(struct vec3 Vec, char Name[])
{
    dbg_printf("\n%s.x = %f\n", Name, fixed_to_double(Vec.x));
    dbg_printf("%s.y = %f\n", Name, fixed_to_double(Vec.y));
    dbg_printf("%s.z = %f\n", Name, fixed_to_double(Vec.z));
}
void debug_vec4(struct vec4 Vec, char Name[])
{
    dbg_printf("\n%s.x = %f\n", Name, fixed_to_double(Vec.x));
    dbg_printf("%s.y = %f\n", Name, fixed_to_double(Vec.y));
    dbg_printf("%s.z = %f\n", Name, fixed_to_double(Vec.z));
    dbg_printf("%s.w = %f\n", Name, fixed_to_double(Vec.w));

}

void debug_matrix4(struct mat4 Matrix, char Name[])
{
    dbg_printf("%s", Name);
    for (int i = 0; i < 4; i++){
        dbg_printf("\n    ");
        for (int j = 0; j < 4; j++){
            dbg_printf("[%d] ", Matrix.m[i][j]);
        }
    }
    dbg_printf("\n");
}
void debug_obj3_vertices(struct obj3 *Obj, char Name[])
{
    dbg_printf("vertices numbers = %d", Obj->vertices_number);
    for (int i = 0; i < Obj->vertices_number; i++){
        dbg_printf("\ni = %d", i);
        debug_vec3(Obj->vertices[i], Name);
    }
}
*/