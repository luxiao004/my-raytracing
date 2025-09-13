#pragma once
#include "triangle.h"
#include "ray.h"

bool ray_triangle_intersect(
    const triangle& triangle,
    const ray& r,
    double& t,
    double& u,
    double& v)
{
    const double EPSILON = 0.0000001;
    vec3 edge1 = triangle.v1 - triangle.v0;
    vec3 edge2 = triangle.v2 - triangle.v0;
    vec3 h = cross(r.direction(), edge2);
    double a = dot(edge1, h);

    if (a > -EPSILON && a < EPSILON)
        return false;    // 射线与三角形平行

    double f = 1.0 / a;
    vec3 s = r.origin() - triangle.v0;
    u = f * dot(s, h);

    if (u < 0.0 || u > 1.0)
        return false;

    vec3 q = cross(s, edge1);
    v = f * dot(r.direction(), q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    // 计算交点距离t
    t = f * dot(edge2, q);
    return t > EPSILON;
}


