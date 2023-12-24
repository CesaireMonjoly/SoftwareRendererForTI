#include <tice.h>
#include <keypadc.h>
#include <graphx.h>
#include <debug.h>

#include <stdlib.h>
#include <math.h>

// GFX_LCD_WIDTH 320
// GFX_LCD_HEIGHT 240
#define MAX_VERTICES 64
#define MAX_TRIANGLES (MAX_VERTICES * 2)

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
    struct vec3 vertices[MAX_VERTICES];
    int triangle_number;
    struct vec3 triangles[MAX_TRIANGLES]; // ve3 {x, y, z} with x, y and z the index of vertices that can form a triangle
    struct vec3 pos;
};
struct cam
{
    int fov;
    struct vec3 pos;
};

struct vec2 screenSpace(struct vec3 CoordinateSpace)
{
    struct vec2 ScreenSpace;
    ScreenSpace.x = CoordinateSpace.x + 160;
    ScreenSpace.y = (CoordinateSpace.y*(-1) + 120);
    return ScreenSpace;
}
struct vec3 coordinateSpace(struct vec3 CamSpace, int Dist)
{
    struct vec3 CoordinateSpace;
    CoordinateSpace.x = (int) CamSpace.x*((CamSpace.z + Dist)/(Dist));
    CoordinateSpace.y = (int) CamSpace.y*((CamSpace.z + Dist)/(Dist));
    CoordinateSpace.z = CamSpace.z;
    return CoordinateSpace;
}

void obj3Move(struct obj3 *Object, struct vec3 Vector)
{
    for (int i = 0; i < Object->vertices_number; i++) {
        Object->vertices[i].x += Vector.x;
        Object->vertices[i].y += Vector.y;
        Object->vertices[i].z += Vector.z;
    }
}
void obj3Process(struct obj3 * Object, struct cam Cam)
{
    gfx_SetColor(255);
    struct vec3 Points;
    struct vec2 ScreenPoints;
    for (int i = 0; i < Object->vertices_number; i++) {
            Points = coordinateSpace(Object->vertices[i], Cam.fov);
            ScreenPoints = screenSpace(Points);
            gfx_SetPixel(ScreenPoints.x, ScreenPoints.y);
        }
}


int GenerateCube(int Size, int Density, struct obj3 * Cube)
{
    int VerticesNumbers =  Density*Density*Density;
    if (VerticesNumbers > MAX_VERTICES){
        return -1;
    }
    Cube->vertices_number = VerticesNumbers;
    for (int i = 0 ; i <= VerticesNumbers; i++)
    {
        Cube->vertices[i].x = (i%Density)*Size + Cube->pos.x;
        Cube->vertices[i].y = ((i/Density)%Density)*Size + Cube->pos.y;
        Cube->vertices[i].z = (i/(Density*Density)*Size) + Cube->pos.z;
        Cube->vertices_number = i;
    }
    return 0;
}

int GenerateCubeObject(int Size, struct obj3 * Cube)
{
    int VerticesNumbers = 8;
    for (int i=0; i < VerticesNumbers; i++) {
        Cube->vertices[i].x = (i & 0x1)*Size;
        Cube->vertices[i].y = ((i & 0x2)/2)*Size;
        Cube->vertices[i].z = ((i & 0x4)/4)*Size;

        dbg_printf("\ni = %d\n", i);
        dbg_printf("x = %d\n", Cube->vertices[i].x);
        dbg_printf("y = %d\n", Cube->vertices[i].y);
        dbg_printf("z = %d\n", Cube->vertices[i].z);
    }
    return 0;
}

void inputMovement(struct vec3 *Movement)
{
    Movement->x = 0;
    Movement->y = 0;
    Movement->z = 0;
    if (kb_Right == kb_Data[7]) {
        Movement->x = 2;
    }
    else if (kb_Left == kb_Data[7]) {
        Movement->x = -2;
    }
    else if (kb_Up == kb_Data[7]){
        Movement->z = 1;
    }
    else if (kb_Down == kb_Data[7]){
        Movement->z = -1;
    }
    else {
        Movement->x = 0;
        Movement->y = 0;
        Movement->z = 0;
    }
}

int main(void)
{
    os_ClrHome();
    gfx_Begin();
    gfx_SetDrawBuffer();

    //Camera
    struct cam Camera = {10,{0, 0, -20}};

    //3D Object(s)
    struct obj3 Cube = {0, {0}, 0, {0}, {50, 0, 0}};
    GenerateCubeObject(8, &Cube);

    //Movement Vector
    struct vec3 MovementVector = {0, 0, 0};

    do {
        inputMovement(&MovementVector);
        obj3Move(&Cube, MovementVector);
        obj3Process(&Cube, Camera);
        gfx_SetColor(0);
        gfx_SwapDraw();
        gfx_ZeroScreen();
    } while (kb_Data[1] != kb_2nd); //exit key

    gfx_ZeroScreen();
    os_ClrHome();
    gfx_End();

    return 0;
}



