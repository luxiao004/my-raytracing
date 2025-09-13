#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"
const double M_PI = 3.14159265358979323846;
class camera {
public:
    camera(
        point3 lookfrom,
        point3 lookat,
        vec3   vup,
        double vfov,          // ¥π÷± ”Ω« (degree)
        double aspect_ratio
    ) {
        auto theta = vfov * M_PI / 180.0;
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
    }

    ray get_ray(double s, double t) const {
        return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
};

#endif
