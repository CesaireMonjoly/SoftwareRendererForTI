#include <tice.h>
# include "math.h"

#define ASPECT_RATIO 1.33333
#define MAX_VERTICES 64
#define MAX_TRIANGLES (MAX_VERTICES * 2)

struct obj3
{
    int vertices_number;
    struct vec3 vertices[MAX_VERTICES];
    int triangle_number;
    struct vec3 triangles[MAX_TRIANGLES]; // vec3 {x, y, z} with x, y and z the index of vertices that can form a triangle
    struct vec3 pos;
    struct vec3 rot;
};
struct cam
{
    fixed_point fov;
    fixed_point aspectRatio;
    fixed_point zNear;
    fixed_point zFar;
    struct vec3 pos;
};