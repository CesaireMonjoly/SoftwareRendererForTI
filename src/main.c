#include <tice.h>
#include <keypadc.h>
#include <graphx.h>
#include <debug.h>
#include <math.h>

#include "type.h"


void buildMatrixProjection(fixed_point Fov, fixed_point AspectRatio, fixed_point ZNear, fixed_point ZFar, struct mat4 *MatrixProjection)
{
    MatrixProjection->m[0][0] = fmul(double_to_fixed(1/tan(fixed_to_double(Fov)/2)), AspectRatio);
    MatrixProjection->m[1][1] = double_to_fixed(1/tan(fixed_to_double(Fov)/2));
    MatrixProjection->m[2][2] = fdiv(ZFar, (ZFar - ZNear) );
    MatrixProjection->m[3][2] = fdiv(fmul(-ZFar, ZNear),(ZFar-ZNear));
    MatrixProjection->m[2][3] = 1;

    debug_matrix4(*MatrixProjection, "Matrix projection");
}

struct vec4 mat4MulVec4(struct mat4 Matrix, struct vec4 Vector)
{
    struct vec4 Output;
    Output.x = fmul(Matrix.m[0][0], Vector.x) + fmul(Matrix.m[0][1],Vector.y) + fmul(Matrix.m[0][2], Vector.z) + fmul(Matrix.m[0][3], Vector.w);
    Output.y = fmul(Matrix.m[1][0], Vector.x) + fmul(Matrix.m[1][1],Vector.y) + fmul(Matrix.m[1][2], Vector.z) + fmul(Matrix.m[1][3], Vector.w);
    Output.z = fmul(Matrix.m[2][0], Vector.x) + fmul(Matrix.m[2][1],Vector.y) + fmul(Matrix.m[2][2], Vector.z) + fmul(Matrix.m[2][3], Vector.w);
    Output.w = fmul(Matrix.m[3][0], Vector.x) + fmul(Matrix.m[3][1],Vector.y) + fmul(Matrix.m[3][2], Vector.z) + fmul(Matrix.m[3][3], Vector.w);
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

struct vec3 reduceVec4(struct vec4 Vector)
{
    struct vec3 Vector3;
    Vector3.x = Vector.x;
    Vector3.y = Vector.y;
    Vector3.z = Vector.z;
    return Vector3;
}

struct vec2 reduceVec3(struct vec3 Vector)
{
    struct vec2 Vector2;
    Vector2.x = Vector.x;
    Vector2.y = Vector.y;
    return Vector2;
}


struct vec2 screenSpace(struct vec2 CoordinateSpace)
{
    struct vec2 ScreenSpace;
    //debug_vec2(CoordinateSpace, "CoordinateSpace");
    ScreenSpace.x = CoordinateSpace.x + 160;
    ScreenSpace.y = fmul(CoordinateSpace.y, -1) + 120;
    //debug_vec2(ScreenSpace, "ScreenSpace");

    return ScreenSpace;
}
struct vec3 ProjectPoint(struct vec3 Point, struct mat4 MatrixProjection)
{
    //debug_vec3(Point, "post");

    struct vec4 Point4 = extendVec3(Point, 1);

    //debug_vec4(Point4, "Point Before");

    Point4 = mat4MulVec4(MatrixProjection, Point4);

    //debug_vec4(Point4, "Point After");

    struct vec3 CoordinateSpace;
    CoordinateSpace.x = fmul(Point4.x, Point4.z);
    CoordinateSpace.y = fmul(Point4.y, Point4.z);
    CoordinateSpace.z = Point4.z;
    return CoordinateSpace;
}

void obj3Move(struct obj3 *Object, struct vec3 Vector)
{
    for (int i = 0; i < Object->vertices_number; i++) {
        debug_vec3(Object->vertices[i], "vertex before");
        Object->vertices[i].x = Object->vertices[i].x + Vector.x;
        Object->vertices[i].y = Object->vertices[i].y + Vector.y;
        Object->vertices[i].z = Object->vertices[i].z + Vector.z;
        debug_vec3(Object->vertices[i], "vertex after");
    }
/*    Object->pos.x += Vector.x;
    Object->pos.y += Vector.y;
    Object->pos.z += Vector.z;*/
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
            Object->vertices[i] = reduceVec4(mat4MulVec4(MatrixRotation[j], Vertex));
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

        //debug_vec3(Object->vertices[k], "triangles");
        /*
        dbg_printf("\nVraiment avant");
        debug_vec3(Object->vertices[Object->triangles[k].x], "vertex.x");
        debug_vec3(Object->vertices[Object->triangles[k].y], "vertex.y");
        debug_vec3(Object->vertices[Object->triangles[k].z], "vertex.z");
*/
        Points[0] = ProjectPoint(Object->vertices[Object->triangles[k].x], MatrixProjection);
        Points[1] = ProjectPoint(Object->vertices[Object->triangles[k].y], MatrixProjection);
        Points[2] = ProjectPoint(Object->vertices[Object->triangles[k].z], MatrixProjection);

        ScreenPoints[0] = screenSpace(reduceVec3(Points[0]));
        ScreenPoints[1] = screenSpace(reduceVec3(Points[1]));
        ScreenPoints[2] = screenSpace(reduceVec3(Points[2]));

/*        dbg_printf("\nAvant :");
        debug_vec2(ScreenPoints[0], "ScreenPoint[0]");
        debug_vec2(ScreenPoints[1], "ScreenPoint[1]");
        debug_vec2(ScreenPoints[2], "ScreenPoint[2]");
*/

        ScreenPoints[0].x = (int) (fixed_to_double(ScreenPoints[0].x));
        ScreenPoints[0].y = (int) (fixed_to_double(ScreenPoints[0].y));
        ScreenPoints[1].x = (int) (fixed_to_double(ScreenPoints[1].x));
        ScreenPoints[1].y = (int) (fixed_to_double(ScreenPoints[1].y));
        ScreenPoints[2].x = (int) (fixed_to_double(ScreenPoints[2].x));
        ScreenPoints[2].y = (int) (fixed_to_double(ScreenPoints[2].y));
/*
        dbg_printf("\nApres :");
        debug_vec2(ScreenPoints[0], "ScreenPoint[0]");
        debug_vec2(ScreenPoints[1], "ScreenPoint[1]");
        debug_vec2(ScreenPoints[2], "ScreenPoint[2]");
*/
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
        debug_vec3(Cube->vertices[i], "Cube");
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
        Movement->z = int_to_fixed(1);
    }
    else if (kb_Div & kb_Data[6]){
        Movement->z = int_to_fixed(-1);
    }

    if (kb_Right & kb_Data[7]){
        Movement->x = int_to_fixed(1);
    }
    else if (kb_Left & kb_Data[7]){
        Movement->x = int_to_fixed(-1);
    }

    if (kb_Up & kb_Data[7]){
        Movement->y = int_to_fixed(1);
    }
    else if (kb_Down & kb_Data[7]){
        Movement->y = int_to_fixed(-1);
    }

}


int main(void)
{
    os_ClrHome();
    gfx_Begin();
    gfx_SetDrawBuffer();

    fixed_point a = double_to_fixed(6.8);
    dbg_printf("%d\n", a);
    double b = fixed_to_double(a);
    dbg_printf("%f\n", b);


    //Camera
    struct cam Camera = {90, ASPECT_RATIO, 3, 50,{0, 0, -20}};

    struct mat4 MatrixProjection= {0};
    buildMatrixProjection(Camera.fov, Camera.aspectRatio, Camera.zNear, Camera.zFar, &MatrixProjection);

    //3D Object(s)
    struct obj3 Cube = {0, {0}, 0, {0},{0, 0, 0}};
    GenerateCubeObject(int_to_fixed(3), &Cube);
    //debug_obj3_vertices(&Cube, "    Cube");

    //Movement Vector
    struct vec3 MovementVector = {int_to_fixed(3), int_to_fixed(10), int_to_fixed(20)};
    //struct vec3 RotVector = {30, 20, 30};
    //obj3Rot(&Cube, RotVector);
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



