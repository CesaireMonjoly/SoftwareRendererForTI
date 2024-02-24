#include <tice.h>
#include <keypadc.h>
#include <graphx.h>
#include <debug.h>
#include <math.h>

#include "type.h"


void buildMatrixProjection(float Fov, float AspectRatio, int ZNear, int ZFar, struct mat4 *MatrixProjection)
{
    MatrixProjection->m[0][0] = 1;//((1/tan(Fov/2)) * AspectRatio)*255;
    MatrixProjection->m[1][1] = 1;//((1/tan(Fov/2)))*255;
    MatrixProjection->m[2][2] = (ZFar/(ZFar - ZNear) );//*255;
    MatrixProjection->m[3][2] = (-ZFar * ZNear)/(ZFar-ZNear);//*255;
    MatrixProjection->m[2][3] = 1;//255;

    debug_matrix4(*MatrixProjection, "Projection");
    /*
     * I'm currently trying to implement a bette matrix projection without using floats
     * The idea is to multiply the whole matrix by 255,
     * and when we multiply the matrix with the vec4 we will have to divide by 255 and round
    */
}

struct vec4 mat4MulVec4(struct mat4 Matrix, struct vec4 Vector)
{
    struct vec4 Output;
    Output.x = Matrix.m[0][0] * Vector.x + Matrix.m[0][1] * Vector.y + Matrix.m[0][2] * Vector.z + Matrix.m[0][3] * Vector.w;
    Output.y = Matrix.m[1][0] * Vector.x + Matrix.m[1][1] * Vector.y + Matrix.m[1][2] * Vector.z + Matrix.m[1][3] * Vector.w;
    Output.z = Matrix.m[2][0] * Vector.x + Matrix.m[2][1] * Vector.y + Matrix.m[2][2] * Vector.z + Matrix.m[2][3] * Vector.w;
    Output.w = Matrix.m[3][0] * Vector.x + Matrix.m[3][1] * Vector.y + Matrix.m[3][2] * Vector.z + Matrix.m[3][3] * Vector.w;
    return Output;
}

struct vec4 extendVec3(struct vec3 Vector, int Value)
{
    struct vec4 Vector4;
    Vector4.x = Vector.x;
    Vector4.y = Vector.y;
    Vector4.z = Vector.z;
    Vector4.w = Value;
    return Vector4;
}

struct vec3 reduce(struct vec4 Vector)
{
    struct vec3 Vector3;
    Vector3.x = Vector.x;
    Vector3.y = Vector.y;
    Vector3.z = Vector.z;
    return Vector3;
}


struct vec2 screenSpace(struct vec3 CoordinateSpace)
{
    struct vec2 ScreenSpace;
    ScreenSpace.x = CoordinateSpace.x + 160;
    ScreenSpace.y = (CoordinateSpace.y*(-1) + 120);
    return ScreenSpace;
}
struct vec3 coordinateSpace(struct vec3 CamSpace, struct mat4 MatrixProjection)
{
    struct vec4 CamSpace4 = extendVec3(CamSpace, 1);

    CamSpace4 = mat4MulVec4(MatrixProjection, CamSpace4);

    /*
    CamSpace4.x = (int)round(CamSpace4.x >> 8);
    CamSpace4.y = (int)round(CamSpace4.y >> 8);
    CamSpace4.z = (int)round(CamSpace4.z >> 8);
    */
    struct vec3 CoordinateSpace;
    CoordinateSpace.x = (int) CamSpace4.x*CamSpace4.z;
    CoordinateSpace.y = (int) CamSpace4.y*CamSpace4.z;
    CoordinateSpace.z = CamSpace4.z;
    return CoordinateSpace;
}

void obj3Move(struct obj3 *Object, struct vec3 Vector)
{
    for (int i = 0; i < Object->vertices_number; i++) {
        Object->vertices[i].x += Vector.x;
        Object->vertices[i].y += Vector.y;
        Object->vertices[i].z += Vector.z;
    }
    Object->pos.x += Vector.x;
    Object->pos.y += Vector.y;
    Object->pos.z += Vector.z;
}

int sine(int angle)
{
    return SIN_LUT[angle];
}

int cosine(int angle)
{
    return SIN_LUT[90-angle];
}

struct mat4 buildRotX(int angle)
{
    int cos = cosine(angle);
    int sin = sine(angle);
    struct mat4 rotX = {{
        {255, 0, 0, 0},
        {0, cos, -sin, 0},
        {0, sin, cos, 0},
        {0, 0, 0, 255}
    }};
    return rotX;
}

struct mat4 buildRotY(int angle)
{
    int cos = cosine(angle);
    int sin = sine(angle);
    struct mat4 rotY = {{
        {cos, 0, sin, 0},
        {0, 255, -0, 0},
        {-sin, 0, cos, 0},
        {0, 0, 0, 255}
    }};
    return rotY;
}

struct mat4 buildRotZ(int angle)
{
        int cos = cosine(angle);
        int sin = sine(angle);
        struct mat4 rotZ = {{
            {cos, -sin, 0, 0},
            {sin, cos, 0, 0},
            {0, 0, 255, 0},
            {0, 0, 0, 255}
        }};
        return rotZ;

}

void obj3Rot(struct obj3 * Object, struct vec3 RotVector)
{
    struct mat4 MatrixRotation[] = {buildRotX(RotVector.x), buildRotY(RotVector.y), buildRotZ(RotVector.z)};
    struct vec3 Pos;
    Pos.x = -Object->pos.x;
    Pos.y = -Object->pos.y;
    Pos.z = -Object->pos.z;
    obj3Move(Object, Pos);
    struct vec4 Vertex;
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < Object->vertices_number; i++) {
            Vertex = extendVec3(Object->vertices[i], 1);
            dbg_printf("\n%d, %d\nAvant :", i, j);
            debug_vec3(Object->vertices[i], "vertex");
            Object->vertices[i] = reduce(mat4MulVec4(MatrixRotation[j], Vertex));
            dbg_printf("L'entre deux :");
            debug_vec3(Object->vertices[i], "vertex");
            Object->vertices[i].x = (Object->vertices[i].x >> 8)+1;
            Object->vertices[i].y = (Object->vertices[i].y >> 8)+1;
            Object->vertices[i].z = (Object->vertices[i].z >> 8)+1;
            dbg_printf("Apres :");
            debug_vec3(Object->vertices[i], "vertex");
        }
    }
    Pos.x = -Object->pos.x;
    Pos.y = -Object->pos.y;
    Pos.z = -Object->pos.z;
    obj3Move(Object, Pos);
}



void obj3Process(struct obj3 *Object, struct mat4 MatrixProjection)
{
    gfx_SetColor(255);
    struct vec3 Points[3];
    struct vec2 ScreenPoints[3];

    for (int k = 0; k < Object->triangle_number; k++){

        Points[0] = coordinateSpace(Object->vertices[Object->triangles[k].x], MatrixProjection);
        Points[1] = coordinateSpace(Object->vertices[Object->triangles[k].y], MatrixProjection);
        Points[2] = coordinateSpace(Object->vertices[Object->triangles[k].z], MatrixProjection);

        ScreenPoints[0] = screenSpace(Points[0]);
        ScreenPoints[1] = screenSpace(Points[1]);
        ScreenPoints[2] = screenSpace(Points[2]);


        gfx_Line(ScreenPoints[0].x, ScreenPoints[0].y, ScreenPoints[1].x, ScreenPoints[1].y);
        gfx_Line(ScreenPoints[1].x, ScreenPoints[1].y, ScreenPoints[2].x, ScreenPoints[2].y);
        gfx_Line(ScreenPoints[2].x, ScreenPoints[2].y, ScreenPoints[0].x, ScreenPoints[0].y);
    }
}

int GenerateCubeObject(int Size, struct obj3 *Cube)
{
    int VerticesNumbers = 9;
    Cube->vertices[0].x = 0;
    Cube->vertices[0].y = 0;
    Cube->vertices[0].z = 0;
    for (int i=0; i < VerticesNumbers; i++) {
        Cube->vertices[i+1].x = (i & 0x1)*Size;
        Cube->vertices[i+1].y = ((i & 0x2)/2)*Size;
        Cube->vertices[i+1].z = ((i & 0x4)/4)*Size;
    }

    //Face 0
    Cube->triangles[0].x = 1;
    Cube->triangles[0].y = 2;
    Cube->triangles[0].z = 4;

    //Face 1
    Cube->triangles[1].x = 5;
    Cube->triangles[1].y = 7;
    Cube->triangles[1].z = 8;

    //Face 2
    Cube->triangles[2].x = 1;
    Cube->triangles[2].y = 3;
    Cube->triangles[2].z = 7;

    //Face 3
    Cube->triangles[3].x = 2;
    Cube->triangles[3].y = 6;
    Cube->triangles[3].z = 8;

    //Face 4
    Cube->triangles[4].x = 1;
    Cube->triangles[4].y = 5;
    Cube->triangles[4].z = 6;

    //Face 5
    Cube->triangles[5].x = 3;
    Cube->triangles[5].y = 4;
    Cube->triangles[5].z = 8;

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

    if (kb_Right & kb_Data[7]){
        Movement->x = 1;
    }
    else if (kb_Left & kb_Data[7]){
        Movement->x = -1;
    }

    if (kb_Up & kb_Data[7]){
        Movement->y = 1;
    }
    else if (kb_Down & kb_Data[7]){
        Movement->y = -1;
    }

}


int main(void)
{
    os_ClrHome();
    gfx_Begin();
    gfx_SetDrawBuffer();

    //Camera
    struct cam Camera = {90, ASPECT_RATIO, 3, 50,{0, 0, -20}};

    struct mat4 MatrixProjection= {0};
    buildMatrixProjection(Camera.fov, Camera.aspectRatio, Camera.zNear, Camera.zFar, &MatrixProjection);

    //3D Object(s)
    struct obj3 Cube = {0, {0}, 0, {0},{0, 0, 0}};
    GenerateCubeObject(3, &Cube);
    //debug_obj3_vertices(&Cube, "    Cube");

    //Movement Vector
    struct vec3 MovementVector = {0, 0, 20};
    struct vec3 RotVector = {30, 20, 30};
    obj3Rot(&Cube, RotVector);
    obj3Move(&Cube, MovementVector);
    MovementVector.z = 0;

    do {
        InputMovement(&MovementVector);
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



