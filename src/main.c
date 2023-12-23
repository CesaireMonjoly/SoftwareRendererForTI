#include <tice.h>
#include <keypadc.h>
#include <graphx.h>
#include <debug.h>

#include <stdlib.h>
#include <math.h>

// GFX_LCD_WIDTH 320
// GFX_LCD_HEIGHT 240
#define MAX_VERTICES_NUMBER 64

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
    struct vec3 vertices[MAX_VERTICES_NUMBER];

};
struct cam
{
    int fov;
    struct vec3 pos;
};

struct vec2 screenSpace(struct vec2 CoordinateSpace)
{
    CoordinateSpace.x = CoordinateSpace.x + 160;
    CoordinateSpace.y = (CoordinateSpace.y*(-1) + 120);
    return CoordinateSpace;
}
struct vec2 coordinateSpace(struct vec3 CamSpace, int Dist)
{
    struct vec2 CoordinateSpace;
    CoordinateSpace.x = CamSpace.x*((CamSpace.z + Dist)/Dist);
    CoordinateSpace.y = CamSpace.y*((CamSpace.z + Dist)/Dist);
    return CoordinateSpace;
}

void obj3Move(struct obj3 *Object, struct vec3 Vector)
{
    for (int i = 0; i < Object->vertices_number; i++)
    {
        Object->vertices[i].x += Vector.x;
        Object->vertices[i].y += Vector.y;
        Object->vertices[i].z += Vector.z;
    }
}
void obj3Process(struct obj3 * Object, struct cam Cam)
{
    gfx_SetColor(255);
    struct vec2 Points;
    for (int i = 0; i < Object->vertices_number; i++)
    {
        Points = coordinateSpace(Object->vertices[i], Cam.fov);
        Points = screenSpace(Points);
        gfx_SetPixel(Points.x, Points.y);
    }
}


int GenerateCube(int Size, int Density, struct obj3 * Cube)
{
    int VerticesNumbers =  Density*Density*Density;
    if (VerticesNumbers > MAX_VERTICES_NUMBER){
        return -1;
    }
    Cube->vertices_number = VerticesNumbers;
    for (int i = 0 ; i < VerticesNumbers; i++)
    {
        Cube->vertices[i].x = (i%Density)*Size;
        Cube->vertices[i].y = ((i/Density)%Density)*Size;
        Cube->vertices[i].z = (i/(Density*Density)*Size);
    }
    return 0;
}

int main(void)
{
    os_ClrHome();
    gfx_Begin();
    gfx_SetDrawBuffer();

    //Camera
    struct cam Camera = {5,{0, 0, 0}};

    //3D Object(s)
    struct obj3 Cube;
    GenerateCube(8, 4, &Cube);

    //Movement Vector
    struct vec3 MovementVector = {-1, 0, 0};

    do
    {
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



