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

void debug_vec3(struct vec3 Obj, char Name[])
{
    dbg_printf("\n%s.x = %d\n", Name, Obj.x);
    dbg_printf("%s.y = %d\n", Name, Obj.y);
    dbg_printf("%s.z = %d\n", Name, Obj.z);
}
void debug_obj3_vertices(struct obj3 *Obj, char Name[])
{
    dbg_printf("vertices numbers = %d", Obj->vertices_number);
    for (int i = 0; i < Obj->vertices_number; i++){
        dbg_printf("\ni = %d", i);
        debug_vec3(Obj->vertices[i], Name);
    }
}

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
    struct vec3 Points[3];
    struct vec2 ScreenPoints[3];

    for (int k = 0; k < Object->triangle_number; k++){
        Points[0] = coordinateSpace(Object->vertices[Object->triangles[k].x], Cam.fov);
        Points[1] = coordinateSpace(Object->vertices[Object->triangles[k].y], Cam.fov);
        Points[2] = coordinateSpace(Object->vertices[Object->triangles[k].z], Cam.fov);

        ScreenPoints[0] = screenSpace(Points[0]);
        ScreenPoints[1] = screenSpace(Points[1]);
        ScreenPoints[2] = screenSpace(Points[2]);

        gfx_Line(ScreenPoints[0].x, ScreenPoints[0].y, ScreenPoints[1].x, ScreenPoints[1].y);
        gfx_Line(ScreenPoints[1].x, ScreenPoints[1].y, ScreenPoints[2].x, ScreenPoints[2].y);
        gfx_Line(ScreenPoints[2].x, ScreenPoints[2].y, ScreenPoints[0].x, ScreenPoints[0].y);
    }
}


int GenerateCubeObject(int Size, struct obj3 * Cube)
{
    int VerticesNumbers = 8;
    for (int i=0; i < VerticesNumbers; i++) {
        Cube->vertices[i].x = (i & 0x1)*Size;
        Cube->vertices[i].y = ((i & 0x2)/2)*Size;
        Cube->vertices[i].z = ((i & 0x4)/4)*Size;
    }

    //Face 0
    Cube->triangles[0].x = 0;
    Cube->triangles[0].y = 1;
    Cube->triangles[0].z = 3;

    //Face 1
    Cube->triangles[1].x = 4;
    Cube->triangles[1].y = 6;
    Cube->triangles[1].z = 7;

    //Face 2
    Cube->triangles[2].x = 0;
    Cube->triangles[2].y = 2;
    Cube->triangles[2].z = 6;

    //Face 3
    Cube->triangles[3].x = 1;
    Cube->triangles[3].y = 5;
    Cube->triangles[3].z = 7;

    //Face 4
    Cube->triangles[4].x = 0;
    Cube->triangles[4].y = 4;
    Cube->triangles[4].z = 5;

    //Face 5
    Cube->triangles[5].x = 2;
    Cube->triangles[5].y = 3;
    Cube->triangles[5].z = 7;

    Cube->triangle_number = 6;
    Cube->vertices_number = VerticesNumbers;
    return 0;
}

void InputMovement(struct vec3 *Movement)
{
    Movement->x = 0;
    Movement->y = 0;
    Movement->z = 0;
    kb_Scan();
    if (kb_Power & kb_Data[6]){
        Movement->z = 1;
    }
    else if (kb_Div & kb_Data[6]){
        Movement->z = -1;
    }
    else if (kb_Right & kb_Data[7]) {
        Movement->x = 1;
    }
    else if (kb_Left & kb_Data[7]) {
        Movement->x = -1;
    }
    else if (kb_Up & kb_Data[7]){
        Movement->y = 1;
    }
    else if (kb_Down & kb_Data[7]){
        Movement->y = -1;
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
    struct obj3 Cube = {0, {0}, 0, {0},{0, 0, 0}};
    GenerateCubeObject(16, &Cube);
    //debug_obj3_vertices(&Cube, "    Cube");

    //Movement Vector
    struct vec3 MovementVector = {0, 0, 20};
    obj3Move(&Cube, MovementVector);
    MovementVector.z = 0;

    do {
        InputMovement(&MovementVector);
        obj3Move(&Cube, MovementVector);
        obj3Process(&Cube, Camera);
        gfx_SetColor(0);
        gfx_SwapDraw();
        gfx_ZeroScreen();
    } while (kb_Data[1] != kb_Clear); //exit key

    gfx_ZeroScreen();
    os_ClrHome();
    gfx_End();

    return 0;
}



