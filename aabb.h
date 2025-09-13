#ifndef AABB_H
#define AABB_H
#include "vec3.h"
#include "ray.h"
#include <algorithm>
class aabb{
public:
	point3 minimum;
	point3 maximum;

	aabb(){}
	aabb(const point3& a,const point3& b):minimum(a),maximum(b){}

	point3 min() const { return minimum; }
	point3 max() const { return maximum; }
	bool hit(const ray& r, double t_min, double t_max)const {
		for (int i = 0;i < 3;++i) {
			//单位距离时间成本
			auto invD = 1.0f / r.direction()[i];
			auto t0 = (min()[i] - r.origin()[i]) * invD;
			auto t1 = (max()[i] - r.origin()[i]) * invD;

			if (invD < 0.0f)std::swap(t0, t1);
			//进入时间取决于最晚进入的时间，离开时间取决于最早离开的时间
			t_min = t0 > t_min ? t0 : t_min;
			t_max = t1 < t_max ? t1 : t_max;
			if (t_max <= t_min)return false;
		}
		return true;
	}

};
inline aabb surrounding_box(const aabb& box0, const aabb& box1) {
		point3 small(fmin(box0.minimum.x(), box1.minimum.x()),
			fmin(box0.minimum.y(), box1.minimum.y()),
			fmin(box0.minimum.z(), box1.minimum.z()));

		point3 big(fmax(box0.maximum.x(), box1.maximum.x()),
			fmax(box0.maximum.y(), box1.maximum.y()),
			fmax(box0.maximum.z(), box1.maximum.z()));

		return aabb(small, big);
	}
#endif
