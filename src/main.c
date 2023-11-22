#include <tice.h>
#include <keypadc.h>
#include <graphx.h>
#include <debug.h>

struct vec2 screenSpace(struct vec2 CoordinateSpace);

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

int main(void)
{
    os_ClrHome();
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetColor(-1);
    //resolution = 320*240
    //int8_t fov = 3;
    //struct vec3 vert = {12, 50, 56};

    struct vec2 pts1 = {-10, -10};
    struct vec2 pts2 = {-60, 60};

    dbg_printf("pts1 = %d, %d\n", pts1.x, pts1.y);
    dbg_printf("pts2 = %d, %d\n", pts2.x, pts2.y);

    pts1 = screenSpace(pts1);
    pts2 = screenSpace(pts2);

    dbg_printf("pts1 = %d, %d\n", pts1.x, pts1.y);
    dbg_printf("pts2 = %d, %d\n", pts2.x, pts2.y);

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



