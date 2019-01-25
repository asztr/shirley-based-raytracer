#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable {
	public:
		sphere() {}
		sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {};
		virtual bool hit(const ray& r, float tmin, float max, hit_record& rec) const;

		vec3 center;
		float radius;
		material *mat_ptr;
};

bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discr = b*b - 4*a*c;
	if (discr > 0) {
		float t_sol[2] = { (-b-sqrt(discr))/(2*a), (-b+sqrt(discr))/(2*a) };
		for (int i=0; i<2; i++) {
			float t_tmp = t_sol[i];
			if (t_tmp < tmax && t_tmp > tmin) {
				rec.t = t_tmp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center) / radius;
				rec.mat_ptr = mat_ptr;
				return true;
			}
		}
	}
	return false;
}

#endif
