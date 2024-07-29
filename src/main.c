#include <tice.h>
#include <keypadc.h>
#include <graphx.h>
#include <debug.h>
#include <math.h>

#include "obj.h"


void buildMatrixProjection(fixed_point Fov, fixed_point AspectRatio, fixed_point ZNear, fixed_point ZFar, struct mat4 *MatrixProjection)
{

/*
    MatrixProjection->m[0][0] = fmul(double_to_fixed(1/tan(fixed_to_double(Fov)/2)), AspectRatio);
    MatrixProjection->m[1][1] = double_to_fixed(1/tan(fixed_to_double(Fov)/2));
    MatrixProjection->m[2][2] = fdiv(ZFar, (ZFar - ZNear));
    MatrixProjection->m[3][2] = fdiv(fmul(-ZFar, ZNear),(ZFar-ZNear));
    MatrixProjection->m[2][3] = int_to_fixed(1);
*/
    MatrixProjection->m[0][0] = int_to_fixed(1);
    MatrixProjection->m[1][1] = int_to_fixed(1);
    MatrixProjection->m[2][2] = int_to_fixed(1);
    MatrixProjection->m[3][2] = int_to_fixed(1);
    MatrixProjection->m[2][3] = int_to_fixed(1);
    //debug_matrix4(*MatrixProjection, "Matrix projection");
}


struct vec2 screenSpace(struct vec2 CoordinateSpace)
{
    struct vec2 ScreenSpace;
    ScreenSpace.x = fixed_to_int((fmul(CoordinateSpace.x, int_to_fixed(1)) + int_to_fixed(160)));
    ScreenSpace.y = fixed_to_int(fmul(CoordinateSpace.y, int_to_fixed(-1)) + int_to_fixed(120));
    return ScreenSpace;
}

struct vec2 ProjectPoint(struct vec3 Point, struct mat4 MatrixProjection)
{
    struct vec4 Point4 = extendVec3(Point, int_to_fixed(1));
    Point4 = mat4MulVec4(MatrixProjection, Point4);
    struct vec2 CoordinateSpace;
    Point4.z = fmul(Point4.z, double_to_fixed(0.02));
    CoordinateSpace.x = (fdiv(Point4.x, Point4.z));
    CoordinateSpace.y = (fdiv(Point4.y, Point4.z));
    return CoordinateSpace;
}

void obj3Move(struct obj3 *Object, struct vec3 Vector)
{
    Object->pos.x += Vector.x;
    Object->pos.y += Vector.y;
    Object->pos.z += Vector.z;
}


struct mat3 buildRotX(int angle)
{
    fixed_point cos = cosine(angle);
    fixed_point sin = sine(angle);
    fixed_point identity = int_to_fixed(1);
    struct mat3 rotX = {{
                                {identity, 0, 0},
                                {0, cos, -sin},
                                {0, sin, cos},
                        }};
    return rotX;
}

struct mat3 buildRotY(int angle)
{
    fixed_point cos = cosine(angle);
    fixed_point sin = sine(angle);
    fixed_point identity = int_to_fixed(1);
    struct mat3 rotY = {{
                                {cos, 0, sin},
                                {0, identity, -0},
                                {-sin, 0, cos},
                        }};
    return rotY;
}

struct mat3 buildRotZ(int angle)
{
    fixed_point cos = cosine(angle);
    fixed_point sin = sine(angle);
    fixed_point identity = int_to_fixed(1);
    struct mat3 rotZ = {{
                                {cos, -sin, 0},
                                {sin, cos, 0},
                                {0, 0, identity},
                        }};
    return rotZ;
}


struct vec3 RotatePoint(struct vec3 Point, struct mat3 Rot){

    Point = mat3MulVec3(Rot, Point);
    return Point;
}



void obj3Process(struct obj3 *Object, struct mat4 MatrixProjection)
{
    gfx_SetColor(255);
    struct vec3 Points[3];
    struct vec2 ScreenPoints[3];

    //dbg_printf("RotX = %d\n", (Object->rot.x));
    struct mat3 RotX = buildRotX(Object->rot.x);

    dbg_printf("RotY = %d\n", (Object->rot.y));
    struct mat3 RotY = buildRotY(Object->rot.y);

    //dbg_printf("RotZ = %d\n", (Object->rot.z));
    struct mat3 RotZ = buildRotZ(Object->rot.z);

    for (int k = 0; k < Object->triangle_number; k++){
        //Retrieves Points related to current triangle (Does three points form a triangle)
        Points[0] = Object->vertices[Object->triangles[k].x];
        Points[1] = Object->vertices[Object->triangles[k].y];
        Points[2] = Object->vertices[Object->triangles[k].z];

        //Rotate Points with rotation matrix
        struct mat3 RotMatrix = mat3Mul(mat3Mul(RotX, RotY), RotZ);
        Points[0] = RotatePoint(Points[0], RotMatrix);
        Points[1] = RotatePoint(Points[1], RotMatrix);
        Points[2] = RotatePoint(Points[2], RotMatrix);

        //Place Points in space with object's position
        Points[0].x += Object->pos.x;
        Points[0].y += Object->pos.y;
        Points[0].z += Object->pos.z;

        Points[1].x += Object->pos.x;
        Points[1].y += Object->pos.y;
        Points[1].z += Object->pos.z;

        Points[2].x += Object->pos.x;
        Points[2].y += Object->pos.y;
        Points[2].z += Object->pos.z;

        //Project Points to Screen
        ScreenPoints[0] = ProjectPoint(Points[0], MatrixProjection);
        ScreenPoints[1] = ProjectPoint(Points[1], MatrixProjection);
        ScreenPoints[2] = ProjectPoint(Points[2], MatrixProjection);

        //Rearrange coordinate to display points on screen
        ScreenPoints[0] = screenSpace(ScreenPoints[0]);
        ScreenPoints[1] = screenSpace(ScreenPoints[1]);
        ScreenPoints[2] = screenSpace(ScreenPoints[2]);

        //Draw Lines on the screen
        //gfx_SetColor(randInt(2, 255)); // Make it funky
        gfx_Line(ScreenPoints[0].x, ScreenPoints[0].y, ScreenPoints[1].x, ScreenPoints[1].y);
        gfx_Line(ScreenPoints[1].x, ScreenPoints[1].y, ScreenPoints[2].x, ScreenPoints[2].y);
        gfx_Line(ScreenPoints[2].x, ScreenPoints[2].y, ScreenPoints[0].x, ScreenPoints[0].y);
    }
}

int GenerateCubeObject(int Size, struct obj3 *Cube)
{
    int VerticesNumbers = 8;
    for (int i=0; i < VerticesNumbers; i++) {
        Cube->vertices[i].x = fmul(int_to_fixed(i & 0x1), Size);
        Cube->vertices[i].y = fmul(int_to_fixed((i & 0x2)>>1), Size);
        Cube->vertices[i].z = fmul(int_to_fixed((i & 0x4)>>2), Size);
        //debug_vec3(Cube->vertices[i], "Cube");
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

void InputMovement(struct vec3 *Movement, struct uint_vec3 *Rotation)
{
    Movement->x = 0;
    Movement->y = 0;
    Movement->z = 0;
    kb_Scan();
    if (kb_Power & kb_Data[6]){
        Movement->z = int_to_fixed(20);
    }
    else if (kb_Div & kb_Data[6]){
        Movement->z = int_to_fixed(-20);
    }

    if (kb_Right & kb_Data[7]){
        Movement->x = int_to_fixed(20);
    }
    else if (kb_Left & kb_Data[7]){
        Movement->x = int_to_fixed(-20);
    }

    if (kb_Up & kb_Data[7]){
        Movement->y = int_to_fixed(20);
    }
    else if (kb_Down & kb_Data[7]){
        Movement->y = int_to_fixed(-20);
    }

    if (kb_4 & kb_Data[3]){
        Rotation->y = (Rotation->y + 3) % 360;
    }
    else if (kb_6 & kb_Data[5]){
        Rotation->y = (Rotation->y + (360 - 3)) % 360;
    }
    if (kb_8 & kb_Data[4]){
        Rotation->x = (Rotation->x + 3) % 360;
    }
    else if (kb_2 & kb_Data[4]){
        Rotation->x = (Rotation->x + (360 - 3)) % 360;
    }
    if (kb_7 & kb_Data[3]){
        Rotation->z = (Rotation->z + 3) % 360;
    }
    else if (kb_9 & kb_Data[5]){
        Rotation->z = (Rotation->z + (360 - 3)) % 360;
    }



}


int main(void)
{
    os_ClrHome();
    gfx_Begin();
    gfx_SetDrawBuffer();

    //Camera
    struct cam Camera = {90, double_to_fixed(ASPECT_RATIO), 3, 50,{0, 0, -20}};

    //Matrix projection
    struct mat4 MatrixProjection= {0};
    buildMatrixProjection(Camera.fov, Camera.aspectRatio, Camera.zNear, Camera.zFar, &MatrixProjection);

    //3D Object(s)
    struct obj3 Cube = {0, {0}, 0, {0},{0, 0, 0}, {0, 0, 0}};
    GenerateCubeObject(int_to_fixed(150), &Cube);

    //Movement Vector
    struct vec3 MovementVector = {int_to_fixed(80), int_to_fixed(60), int_to_fixed(200)};
    obj3Move(&Cube, MovementVector);

    do {
        InputMovement(&MovementVector, &Cube.rot);
        obj3Move(&Cube, MovementVector);

        obj3Process(&Cube, MatrixProjection);
        gfx_SetColor(0);
        gfx_SwapDraw();
        gfx_ZeroScreen();
    } while (kb_Data[1] != kb_Clear); //exit key

    gfx_ZeroScreen();
    os_ClrHome();
    gfx_End();

    return 0;
}



