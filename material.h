#ifndef MATERIAL_H
#define MATERIAL_H
#include "ray.h"
#include "hittable.h"

struct hit_record;
class material{
public:
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation ,ray& sacttered
	)const = 0;
	virtual color emitted()const {
		return color(0, 0, 0);
	}
};

class lambertian :public material {
public:
	color albedo;
	lambertian(const color& a) :albedo(a) {}
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	)const override {
		auto scatter_direction = rec.normal + random_unit_vector();
		if (near_zero(scatter_direction))
			scatter_direction = rec.normal;
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}
};

class metal :public material {
public:
	color albedo;
	double fuzz;

	metal(const color& a, double f) :albedo(a), fuzz(f < 1 ? f : 1) {}
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	)const override {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
};
// 在 material.h 中，新增：
class diffuse_light : public material {
public:
	color emit;
	diffuse_light() {}
	diffuse_light(const color& c) : emit(c) {}
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		return false; // 发光体不散射（路径追踪只取 emitted）
	}
	virtual color emitted() const override {
		return emit;
	}
};

#endif
