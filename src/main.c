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

void applyRotationZ(struct vec3 Origin, struct vec3*Point, int Angle)
{
    double dx = (Point->x - Origin.x);
    double dy = (Point->y - Origin.y);
    double dist = sqrt(dx*dx + dy*dy);
    dx = cos(Angle) * (dist);
    dy = sin(Angle) * (dist);
    Point->x = (int) round((double) Origin.x + dx);
    Point->y = (int) round((double) Origin.y + dy);
}

void applyRotationX(struct vec3 Origin, struct vec3*Point, int Angle)
{
    double dz = (Point->z - Origin.z);
    double dy = (Point->y - Origin.y);
    double dist = sqrt(dz*dz + dy*dy);
    dz = cos(Angle) * (dist);
    dy = sin(Angle) * (dist);
    Point->z = (int) round((double) Origin.x + dz);
    Point->y = (int) round((double) Origin.y + dy);
}

void applyRotationY(struct vec3 Origin, struct vec3*Point, int Angle)
{
    double dx = (Point->x - Origin.x);
    double dz = (Point->z - Origin.z);
    double dist = sqrt(dx*dx + dz*dz);
    dx = cos(Angle) * (dist);
    dz = sin(Angle) * (dist);
    Point->x = (int) round((double) Origin.x + dx);
    Point->z = (int) round((double) Origin.z + dz);
}

void drawCube(int Dist, struct vec3 MovementVector)
{
    struct vec3 CamSpace = {0, 0, 0};
    struct vec3 Origin = {0, 0, 0};
    struct vec2 CoordinateSpace;
    struct vec2 ScreenSpace;
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
                applyMovement(Origin, &CamSpace, MovementVector);
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
    gfx_SetColor(255);
    int Fov = 7;
    struct vec3 Origine = {0, 0, 0};
    struct vec3 Point = {10, 6, 9};

    struct vec2 Origine2D;
    struct vec2 Point2D;
    struct vec3 Move = {1, 0, 0};

    do
    {
        gfx_SetColor(255);
        drawCube(Fov, Move);

        Origine2D = coordinateSpace(Origine, Fov);
        Origine2D = screenSpace(Origine2D);
        Point2D = coordinateSpace(Point, Fov);
        Point2D = screenSpace(Point2D);
        gfx_SetPixel(Point2D.x, Point2D.y);
        gfx_SetPixel(Origine2D.x, Origine2D.y);

        gfx_SetColor(0);
        gfx_SwapDraw();
        gfx_ZeroScreen();
    } while (kb_Data[1] != kb_2nd); //exit key

    gfx_ZeroScreen();
    os_ClrHome();
    gfx_End();

    return 0;
}



