#include <tice.h>

#define ASPECT_RATIO 1.333333
#define MAX_VERTICES 64
#define MAX_TRIANGLES (MAX_VERTICES * 2)


struct mat4
{
    int m[4][4];
};

struct vec4
{
    int x;
    int y;
    int z;
    int w;
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
// Formula: sin(2*pi*t/T)
const int16_t SIN_LUT[361] = {
        0x0000,0x0004,0x0009,0x000D,0x0012,0x0016,0x001B,
        0x001F,0x0023,0x0028,0x002C,0x0031,0x0035,0x0039,
        0x003E,0x0042,0x0046,0x004A,0x004F,0x0053,0x0057,
        0x005B,0x005F,0x0063,0x0067,0x006B,0x006F,0x0073,
        0x0077,0x007B,0x007F,0x0083,0x0087,0x008B,0x008E,
        0x0092,0x0096,0x0099,0x009D,0x00A0,0x00A4,0x00A7,
        0x00AA,0x00AE,0x00B1,0x00B4,0x00B7,0x00BA,0x00BD,
        0x00C0,0x00C3,0x00C6,0x00C9,0x00CB,0x00CE,0x00D0,
        0x00D3,0x00D5,0x00D8,0x00DA,0x00DC,0x00DF,0x00E1,
        0x00E3,0x00E5,0x00E7,0x00E9,0x00EA,0x00EC,0x00EE,
        0x00EF,0x00F1,0x00F2,0x00F4,0x00F5,0x00F6,0x00F7,
        0x00F8,0x00F9,0x00FA,0x00FB,0x00FC,0x00FC,0x00FD,
        0x00FD,0x00FE,0x00FE,0x00FF,0x00FF,0x00FF,0x00FF,
        0x00FF,0x00FF,0x00FF,0x00FE,0x00FE,0x00FE,0x00FD,
        0x00FD,0x00FC,0x00FB,0x00FB,0x00FA,0x00F9,0x00F8,
        0x00F7,0x00F5,0x00F4,0x00F3,0x00F2,0x00F0,0x00EF,
        0x00ED,0x00EB,0x00EA,0x00E8,0x00E6,0x00E4,0x00E2,
        0x00E0,0x00DE,0x00DB,0x00D9,0x00D7,0x00D4,0x00D2,
        0x00CF,0x00CD,0x00CA,0x00C7,0x00C4,0x00C2,0x00BF,
        0x00BC,0x00B9,0x00B5,0x00B2,0x00AF,0x00AC,0x00A9,
        0x00A5,0x00A2,0x009E,0x009B,0x0097,0x0094,0x0090,
        0x008C,0x0089,0x0085,0x0081,0x007D,0x0079,0x0075,
        0x0071,0x006D,0x0069,0x0065,0x0061,0x005D,0x0059,
        0x0055,0x0051,0x004C,0x0048,0x0044,0x0040,0x003B,
        0x0037,0x0033,0x002E,0x002A,0x0026,0x0021,0x001D,
        0x0018,0x0014,0x0010,0x000B,0x0007,0x0002,0xFFFE,
        0xFFF9,0xFFF5,0xFFF0,0xFFEC,0xFFE8,0xFFE3,0xFFDF,
        0xFFDA,0xFFD6,0xFFD2,0xFFCD,0xFFC9,0xFFC5,0xFFC0,
        0xFFBC,0xFFB8,0xFFB4,0xFFAF,0xFFAB,0xFFA7,0xFFA3,
        0xFF9F,0xFF9B,0xFF97,0xFF93,0xFF8F,0xFF8B,0xFF87,
        0xFF83,0xFF7F,0xFF7B,0xFF77,0xFF74,0xFF70,0xFF6C,
        0xFF69,0xFF65,0xFF62,0xFF5E,0xFF5B,0xFF57,0xFF54,
        0xFF51,0xFF4E,0xFF4B,0xFF47,0xFF44,0xFF41,0xFF3E,
        0xFF3C,0xFF39,0xFF36,0xFF33,0xFF31,0xFF2E,0xFF2C,
        0xFF29,0xFF27,0xFF25,0xFF22,0xFF20,0xFF1E,0xFF1C,
        0xFF1A,0xFF18,0xFF16,0xFF15,0xFF13,0xFF11,0xFF10,
        0xFF0E,0xFF0D,0xFF0C,0xFF0B,0xFF09,0xFF08,0xFF07,
        0xFF06,0xFF05,0xFF05,0xFF04,0xFF03,0xFF03,0xFF02,
        0xFF02,0xFF02,0xFF01,0xFF01,0xFF01,0xFF01,0xFF01,
        0xFF01,0xFF01,0xFF02,0xFF02,0xFF03,0xFF03,0xFF04,
        0xFF04,0xFF05,0xFF06,0xFF07,0xFF08,0xFF09,0xFF0A,
        0xFF0B,0xFF0C,0xFF0E,0xFF0F,0xFF11,0xFF12,0xFF14,
        0xFF16,0xFF17,0xFF19,0xFF1B,0xFF1D,0xFF1F,0xFF21,
        0xFF24,0xFF26,0xFF28,0xFF2B,0xFF2D,0xFF30,0xFF32,
        0xFF35,0xFF37,0xFF3A,0xFF3D,0xFF40,0xFF43,0xFF46,
        0xFF49,0xFF4C,0xFF4F,0xFF52,0xFF56,0xFF59,0xFF5C,
        0xFF60,0xFF63,0xFF67,0xFF6A,0xFF6E,0xFF72,0xFF75,
        0xFF79,0xFF7D,0xFF81,0xFF85,0xFF89,0xFF8D,0xFF91,
        0xFF95,0xFF99,0xFF9D,0xFFA1,0xFFA5,0xFFA9,0xFFAD,
        0xFFB1,0xFFB6,0xFFBA,0xFFBE,0xFFC2,0xFFC7,0xFFCB,
        0xFFCF,0xFFD4,0xFFD8,0xFFDD,0xFFE1,0xFFE5,0xFFEA,
        0xFFEE,0xFFF3,0xFFF7,0xFFFC };


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
void debug_vec4(struct vec4 Obj, char Name[])
{
    dbg_printf("\n%s.x = %d\n", Name, Obj.x);
    dbg_printf("%s.y = %d\n", Name, Obj.y);
    dbg_printf("%s.z = %d\n", Name, Obj.z);
    dbg_printf("%s.w = %d\n", Name, Obj.w);

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
