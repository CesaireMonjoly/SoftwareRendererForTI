#include "fixed.h"

#define ASPECT_RATIO 1.33333
#define MAX_VERTICES 64
#define MAX_TRIANGLES (MAX_VERTICES * 2)


struct mat4
{
    fixed_point m[4][4];
};

struct mat3
{
    fixed_point m[3][3];
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
/*
void buildSINLUT(){
    dbg_printf("const fixed_point SIN_LUT[361] = {");
    for (int i = 0; i < 361; i++){
        dbg_printf("%#08x,",double_to_fixed(sin(i * (M_PI/180))));
        if ((i+1)%10 == 0){
            dbg_printf("\n");
        }
    }
    dbg_printf("\n};");
}
 */

const fixed_point SIN_LUT[361] = {00000000,0x000001,0x000002,0x000003,0x000004,0x000005,0x000006,0x000007,0x000008,0x00000a,
                                  0x00000b,0x00000c,0x00000d,0x00000e,0x00000f,0x000010,0x000011,0x000012,0x000013,0x000014,
                                  0x000015,0x000016,0x000017,0x000019,0x00001a,0x00001b,0x00001c,0x00001d,0x00001e,0x00001f,
                                  0x000020,0x000020,0x000021,0x000022,0x000023,0x000024,0x000025,0x000026,0x000027,0x000028,
                                  0x000029,0x000029,0x00002a,0x00002b,0x00002c,0x00002d,0x00002e,0x00002e,0x00002f,0x000030,
                                  0x000031,0x000031,0x000032,0x000033,0x000033,0x000034,0x000035,0x000035,0x000036,0x000036,
                                  0x000037,0x000037,0x000038,0x000039,0x000039,0x00003a,0x00003a,0x00003a,0x00003b,0x00003b,
                                  0x00003c,0x00003c,0x00003c,0x00003d,0x00003d,0x00003d,0x00003e,0x00003e,0x00003e,0x00003e,
                                  0x00003f,0x00003f,0x00003f,0x00003f,0x00003f,0x00003f,0x00003f,0x00003f,0x00003f,0x00003f,
                                  0x000040,0x00003f,0x00003f,0x00003f,0x00003f,0x00003f,0x00003f,0x00003f,0x00003f,0x00003f,
                                  0x00003f,0x00003e,0x00003e,0x00003e,0x00003e,0x00003d,0x00003d,0x00003d,0x00003c,0x00003c,
                                  0x00003c,0x00003b,0x00003b,0x00003a,0x00003a,0x00003a,0x000039,0x000039,0x000038,0x000037,
                                  0x000037,0x000036,0x000036,0x000035,0x000035,0x000034,0x000033,0x000033,0x000032,0x000031,
                                  0x000031,0x000030,0x00002f,0x00002e,0x00002e,0x00002d,0x00002c,0x00002b,0x00002a,0x000029,
                                  0x000029,0x000028,0x000027,0x000026,0x000025,0x000024,0x000023,0x000022,0x000021,0x000020,
                                  0x000020,0x00001f,0x00001e,0x00001d,0x00001c,0x00001b,0x00001a,0x000019,0x000017,0x000016,
                                  0x000015,0x000014,0x000013,0x000012,0x000011,0x000010,0x00000f,0x00000e,0x00000d,0x00000c,
                                  0x00000b,0x00000a,0x000008,0x000007,0x000006,0x000005,0x000004,0x000003,0x000002,0x000001,
                                  00000000,0xffffff,0xfffffe,0xfffffd,0xfffffc,0xfffffb,0xfffffa,0xfffff9,0xfffff8,0xfffff6,
                                  0xfffff5,0xfffff4,0xfffff3,0xfffff2,0xfffff1,0xfffff0,0xffffef,0xffffee,0xffffed,0xffffec,
                                  0xffffeb,0xffffea,0xffffe9,0xffffe7,0xffffe6,0xffffe5,0xffffe4,0xffffe3,0xffffe2,0xffffe1,
                                  0xffffe1,0xffffe0,0xffffdf,0xffffde,0xffffdd,0xffffdc,0xffffdb,0xffffda,0xffffd9,0xffffd8,
                                  0xffffd7,0xffffd7,0xffffd6,0xffffd5,0xffffd4,0xffffd3,0xffffd2,0xffffd2,0xffffd1,0xffffd0,
                                  0xffffcf,0xffffcf,0xffffce,0xffffcd,0xffffcd,0xffffcc,0xffffcb,0xffffcb,0xffffca,0xffffca,
                                  0xffffc9,0xffffc9,0xffffc8,0xffffc7,0xffffc7,0xffffc6,0xffffc6,0xffffc6,0xffffc5,0xffffc5,
                                  0xffffc4,0xffffc4,0xffffc4,0xffffc3,0xffffc3,0xffffc3,0xffffc2,0xffffc2,0xffffc2,0xffffc2,
                                  0xffffc1,0xffffc1,0xffffc1,0xffffc1,0xffffc1,0xffffc1,0xffffc1,0xffffc1,0xffffc1,0xffffc1,
                                  0xffffc0,0xffffc1,0xffffc1,0xffffc1,0xffffc1,0xffffc1,0xffffc1,0xffffc1,0xffffc1,0xffffc1,
                                  0xffffc1,0xffffc2,0xffffc2,0xffffc2,0xffffc2,0xffffc3,0xffffc3,0xffffc3,0xffffc4,0xffffc4,
                                  0xffffc4,0xffffc5,0xffffc5,0xffffc6,0xffffc6,0xffffc6,0xffffc7,0xffffc7,0xffffc8,0xffffc9,
                                  0xffffc9,0xffffca,0xffffca,0xffffcb,0xffffcb,0xffffcc,0xffffcd,0xffffcd,0xffffce,0xffffcf,
                                  0xffffcf,0xffffd0,0xffffd1,0xffffd2,0xffffd2,0xffffd3,0xffffd4,0xffffd5,0xffffd6,0xffffd7,
                                  0xffffd7,0xffffd8,0xffffd9,0xffffda,0xffffdb,0xffffdc,0xffffdd,0xffffde,0xffffdf,0xffffe0,
                                  0xffffe0,0xffffe1,0xffffe2,0xffffe3,0xffffe4,0xffffe5,0xffffe6,0xffffe7,0xffffe9,0xffffea,
                                  0xffffeb,0xffffec,0xffffed,0xffffee,0xffffef,0xfffff0,0xfffff1,0xfffff2,0xfffff3,0xfffff4,
                                  0xfffff5,0xfffff6,0xfffff8,0xfffff9,0xfffffa,0xfffffb,0xfffffc,0xfffffd,0xfffffe,0xffffff,
                                  00000000
};


struct vec4 mat4MulVec4(struct mat4 Matrix, struct vec4 Vector)
{
    struct vec4 Output;
    Output.x = fmul(Matrix.m[0][0], Vector.x) + fmul(Matrix.m[0][1],Vector.y) + fmul(Matrix.m[0][2], Vector.z) + fmul(Matrix.m[0][3], Vector.w);
    Output.y = fmul(Matrix.m[1][0], Vector.x) + fmul(Matrix.m[1][1],Vector.y) + fmul(Matrix.m[1][2], Vector.z) + fmul(Matrix.m[1][3], Vector.w);
    Output.z = fmul(Matrix.m[2][0], Vector.x) + fmul(Matrix.m[2][1],Vector.y) + fmul(Matrix.m[2][2], Vector.z) + fmul(Matrix.m[2][3], Vector.w);
    Output.w = fmul(Matrix.m[3][0], Vector.x) + fmul(Matrix.m[3][1],Vector.y) + fmul(Matrix.m[3][2], Vector.z) + fmul(Matrix.m[3][3], Vector.w);
    return Output;
}
struct vec3 mat3MulVec3(struct mat3 Matrix, struct vec3 Vector)
{
    struct vec3 Output;
    Output.x = fmul(Matrix.m[0][0], Vector.x) + fmul(Matrix.m[0][1],Vector.y) + fmul(Matrix.m[0][2], Vector.z);
    Output.y = fmul(Matrix.m[1][0], Vector.x) + fmul(Matrix.m[1][1],Vector.y) + fmul(Matrix.m[1][2], Vector.z);
    Output.z = fmul(Matrix.m[2][0], Vector.x) + fmul(Matrix.m[2][1],Vector.y) + fmul(Matrix.m[2][2], Vector.z);
    return Output;
}

int sine(int angle)
{
    return SIN_LUT[angle];
}

int cosine(int angle)
{
    return SIN_LUT[90-angle];
}


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
void debug_matrix3(struct mat3 Matrix, char Name[])
{
    dbg_printf("%s", Name);
    for (int i = 0; i < 3; i++){
        dbg_printf("\n    ");
        for (int j = 0; j < 3; j++){
            dbg_printf("[%d] ", Matrix.m[i][j]);
        }
    }
    dbg_printf("\n");
}


