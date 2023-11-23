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

void drawCube(int dist, int xpos)
{
    struct vec3 CamSpace = {0, 0, 0};
    struct vec2 CoordinateSpace = {0, 0};
    struct vec2 ScreenSpace = {0, 0};
    int color_index = 7;
    for (int x = 0; x < 4 ; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            for (int z = 0; z < 4; z++)
            {
                color_index = color_index + 8;
                gfx_SetColor(color_index);
                CamSpace.x = (x*4)-xpos;
                CamSpace.y = (y*4)-4;
                CamSpace.z = (z*4);
                CoordinateSpace = coordinateSpace(CamSpace, dist);
                ScreenSpace = screenSpace(CoordinateSpace);
                gfx_SetPixel(ScreenSpace.x, ScreenSpace.y);
            }
        }
    }
}


int main(void)
{
    //resolution = 320*240

    os_ClrHome();
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetColor(0);
    int fov = 5;
    uint8_t xpos = 60;

    do
    {
        gfx_ZeroScreen();
        drawCube(fov, xpos);
        xpos = xpos + 1;
        //gfx_Line(pts1.x, pts1.y, pts2.x, pts2.y);
        gfx_SetColor(0);
        gfx_SwapDraw();
    } while (kb_Data[1] != kb_2nd); //exit key

    gfx_ZeroScreen();
    os_ClrHome();
    gfx_End();

    return 0;
}



