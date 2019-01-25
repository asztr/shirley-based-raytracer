#ifndef DISCH
#define DISCH

#include "hitable.h"

using namespace std;

class disc: public hitable {
	public:
		disc() {}
		disc(vec3 cen, vec3 n, float r) : center(cen), normal(n), radius(r) {};
		virtual bool hit(const ray& r, float tmin, float max, hit_record& rec) const;

		vec3 center;
		vec3 normal;
		float radius;
};

bool disc::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
	float tmp1 = dot(center - r.origin(), normal);
	float tmp2 = dot(r.direction(), normal);
	//cout << center << " | " << r.origin() << "| " << normal << " | " << tmp1 << " | " << tmp2 << endl;
	if (tmp2 != 0.0) {
		float t = tmp1/tmp2;
		if ((t > tmin) && (t < tmax)) {
			vec3 p = r.point_at_parameter(t);
			if ((p - center).length() < radius) {
				//cout << t << endl;
				rec.t = t;
				rec.p = p;
				rec.normal = normal;
				//float sign = tmp2/abs(tmp2);
				//rec.normal = sign * normal;
				return true;
			}
		}
	return false;
	}
}

#endif
