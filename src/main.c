#include <tice.h>
#include <keypadc.h>
#include <graphx.h>

void bresenham(int8_t x0,int8_t y0,int8_t x1,int8_t y1);


struct vec3
{
    int8_t x;
    int8_t y;
    int8_t z;
};

struct vec2
{
    int8_t x;
    int8_t y;
};

int main(void)
{
    os_ClrHome();
    gfx_Begin();
    gfx_SetDrawBuffer();

    
    //resolution = 320*240
    int8_t fov = 3;
    struct vec3 vert = {12, 50, 56};
    
    
    struct vec2 pts1 = {10, 10};
    struct vec2 pts2 = {60, 120};
    gfx_SetColor(0);


    bresenham(pts1.x, pts1.y, pts2.x, pts2.y);

    do
    {
        gfx_SetColor(0);
        gfx_SetPixel(0, 0);
        gfx_SwapDraw();
    
    } while (kb_Data[1] != kb_2nd); //exit key

    gfx_ZeroScreen();
    os_ClrHome();
    gfx_End();

    return 0;
}

void bresenham(int8_t x0,int8_t y0,int8_t x1,int8_t y1)
{
	int8_t Distance_Y = y1-y0;
	int8_t Distance_X = x1-x0;

    int8_t X_screen = x0;
    int8_t Y_screen = y0;

    int8_t p = 2*Distance_X-Distance_Y;

    while(X_screen<x1)
    {
        if(p>=0)
        {
            gfx_SetPixel(X_screen, Y_screen);
            Y_screen=Y_screen+1;
            p=p+2*Distance_Y-2*Distance_X;
        }
        else
        {
            gfx_SetPixel(X_screen, Y_screen);
            p=p+2*Distance_Y-2*Distance_X;
        }
        X_screen=X_screen+1;
    }
    gfx_SwapDraw();
}


