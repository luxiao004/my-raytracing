#ifndef TRIANGLE_HITTABLE_H
#define TRIANGLE_HITTABLE_H
#include "hittable.h"

class triangle_hittable :public hittable {
public:
	point3 v0, v1, v2;
	vec3 normal;

	triangle_hittable(const point3& a, const point3& b, const point3& c)
		:v0(a), v1(b), v2(c) {
		normal = unit_vector(cross(v1 - v0, v2 - v0));
	}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const override {
		//Möller–Trumbore intersection algorithm
		const double EPSILON = 1e-8;
		vec3 E1 = v1 - v0;
		vec3 E2 = v2 - v0;
		vec3 S1 = cross(r.direction(), E2);
		double a = dot(S1, E1);
		if (fabs(a) < EPSILON)return false;

		double f = 1.0 / a;
		vec3 S = r.origin() - v0;
		double u = f * dot(S, S1);
		if (u < 0.0 || u>1.0)return false;

		vec3 S2 = cross(S, E1);
		double v = f * dot(r.direction(), S2);
		if (v < 0.0 || u + v>1.0)return false;
		
		
		double t = f * dot(E2, S2);
		if (t< t_min || t>t_max)return false;

		rec.t = t;
		rec.p = r.at(t);
		rec.set_face_normal(r, normal);
		return true;
	}
	virtual bool bounding_box(double time0, double time1, aabb& output_box)const override {
		point3 minpt(fmin(fmin(v0.x(),v1.x()),v2.x()),
					fmin(fmin(v0.y(),v1.y()),v2.y()),
					fmin(fmin(v0.z(),v1.z()),v2.z()));
		point3 maxpt(fmax(fmax(v0.x(), v1.x()), v2.x()),
					fmax(fmax(v0.y(), v1.y()), v2.y()),
					fmax(fmax(v0.z(), v1.z()), v2.z()));
		const double eps = 1e-4;
		output_box = aabb(minpt - vec3(eps, eps, eps), maxpt + vec3(eps, eps, eps));
		return true;
	}
};
#endif
