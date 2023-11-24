#include <tice.h>
#include <keypadc.h>
#include <graphx.h>
#include <debug.h>

#include <stdlib.h>
#include <math.h>

//GFX_LCD_WIDTH
//GFX_LCD_HEIGHT

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

struct vec2 coordinateSpace(struct vec3 CamSpace, int Dist)
{
    struct vec2 CoordinateSpace;
    CoordinateSpace.x = CamSpace.x*((CamSpace.z + Dist)/Dist);
    CoordinateSpace.y = CamSpace.y*((CamSpace.z + Dist)/Dist);
    return CoordinateSpace;
}

void applyScale(struct vec3 Origin, struct vec3*Point, struct vec3 ScaleVector)
{
    int dx = abs(Origin.x - Point->x)*ScaleVector.x;
    int dy = abs(Origin.x - Point->y)*ScaleVector.y;
    int dz = abs(Origin.z - Point->z)*ScaleVector.z;

    Point->x = Point->x * dx;
    Point->y = Point->y * dy;
    Point->z = Point->z * dz;
}

void applyMovement(struct vec3 Origin, struct vec3*Point, struct vec3 MovementVector)
{
    int dx = abs(Origin.x - Point->x)*MovementVector.x;
    int dy = abs(Origin.x - Point->y)*MovementVector.y;
    int dz = abs(Origin.z - Point->z)*MovementVector.z;

    Point->x = Point->x + dx;
    Point->y = Point->y + dy;
    Point->z = Point->z + dz;
}

void drawCube(int Dist, int Angle)
{
    struct vec3 CamSpace = {0, 0, 0};
    struct vec2 CoordinateSpace = {0, 0};
    struct vec2 ScreenSpace = {0, 0};
    int Color_index = 7;
    for (int x = -2; x < 3 ; x++)
    {
        for (int y = -2; y < 3; y++)
        {
            Color_index = Color_index + 8;
            gfx_SetColor(Color_index);
            for (int z = 0; z < 4; z++)
            {
                CamSpace.x = x*8;
                CamSpace.y = y*8;
                CamSpace.z = z*8;


                CoordinateSpace = coordinateSpace(CamSpace, Dist);
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
    int Fov = 7;
    int Angle = 0;

    do
    {
        gfx_ZeroScreen();
        Angle += 0;
        drawCube(Fov, Angle);
        gfx_SetColor(0);
        gfx_SwapDraw();
    } while (kb_Data[1] != kb_2nd); //exit key

    gfx_ZeroScreen();
    os_ClrHome();
    gfx_End();

    return 0;
}



