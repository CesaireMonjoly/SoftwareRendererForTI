#include <tice.h>
#include <keypadc.h>
#include <graphx.h>
#include <debug.h>

struct vec3
{
    int24_t x;
    int24_t y;
    int24_t z;
};

struct vec2
{
    int24_t x;
    int24_t y;
};

struct vec2 screenSpace(struct vec2 CoordinateSpace)
{
    CoordinateSpace.x = CoordinateSpace.x + 160;
    CoordinateSpace.y = (CoordinateSpace.y*(-1) + 120);
    return CoordinateSpace;
}

struct vec2 coordinateSpace(struct vec3 CamSpace, int dist)
{
    struct vec2 CoordinateSpace;
    CoordinateSpace.x = CamSpace.x*((CamSpace.z + dist)/dist);
    CoordinateSpace.y = CamSpace.y*((CamSpace.z + dist)/dist);
    return CoordinateSpace;
}

int main(void)
{
    //resolution = 320*240

    os_ClrHome();
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetColor(-1);
    int fov = 3;
    struct vec3 vert1 = {10, 10, 3};
    struct vec3 vert2 = {10, 10, 2};

    dbg_printf("vert1 = %d, %d, %d\n",vert1.x, vert1.y, vert1.z);
    dbg_printf("vert1 = %d, %d, %d\n",vert2.x, vert2.y, vert2.z);

    struct vec2 pts1 = coordinateSpace(vert1, fov);
    struct vec2 pts2 = coordinateSpace(vert2, fov);

    dbg_printf("pts1 = %d, %d\n",pts1.x, pts1.y);
    dbg_printf("pts2 = %d, %d\n",pts2.x, pts2.y);

    pts1 = screenSpace(pts1);
    pts2 = screenSpace(pts2);


    do
    {
        gfx_Line(pts1.x, pts1.y, pts2.x, pts2.y);
        gfx_SetColor(0);
        gfx_SetPixel(0, 0);
        gfx_SwapDraw();
    } while (kb_Data[1] != kb_2nd); //exit key

    gfx_ZeroScreen();
    os_ClrHome();
    gfx_End();

    return 0;
}



