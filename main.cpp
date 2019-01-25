#include <iostream>
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "disc.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
//#include "material.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>

#include "geometry.h"

using namespace std;

int envmap_width, envmap_height;
vector<Vec3f> envmap;

vec3 sky_color(const ray& r) {
	vec3 unit_direction = unit_vector(r.direction());
	//float t = 0.5 + 0.5*unit_direction.y();
	//return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);

	float i = (envmap_width/2)*(1.0 + unit_direction[0])-1.0;
	float j = (envmap_height/2)*(1.0 + unit_direction[1])-1.0;
	Vec3f col = envmap[int(i)+int(j)*envmap_width];
	vec3 ret(col[0], col[1], col[2]);
	return ret;
}

vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 1e-3, MAXFLOAT, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation*color(scattered, world, depth+1);
		else
			return vec3(0,0,0);
		//vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		//return 0.5*color(ray(rec.p, target-rec.p), world);
		//return 0.5*vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
	}
	else 
		return sky_color(r);
}

int main() {
	int n = -1;
	unsigned char *pixmap = stbi_load("lightprobe.jpg", &envmap_width, &envmap_height, &n, 0);
	if (!pixmap || 3!=n) {
		std::cerr << "Error: can not load the environment map" << std::endl;
		return -1;
	}
	envmap = std::vector<Vec3f>(envmap_width*envmap_height);
	for (int j = envmap_height-1; j>=0 ; j--) {
		for (int i = 0; i<envmap_width; i++) {
			envmap[i+j*envmap_width] = Vec3f(pixmap[(i+j*envmap_width)*3+0], pixmap[(i+j*envmap_width)*3+1], pixmap[(i+j*envmap_width)*3+2])*(1/255.);
		}
	}
	stbi_image_free(pixmap);

	int nx = 1024;
	int ny = 512;
	int ns = 100;
	cout << "P3\n" << nx << " " << ny << "\n255\n";
	hitable *list[4];
	list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.8,0.3,0.3)));
	list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8,0.8,0)));
	list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8,0.6,0.2)));
	list[3] = new sphere(vec3(-1,0,-1), 0.5, new metal(vec3(0.8,0.8,0.8)));
	//list[0] = new sphere(vec3(0,0,-1), 0.5);
	//list[1] = new sphere(vec3(0,-100.5,-1), 1);
	//list[1] = new disc(vec3(0,-0.5,-1.5), vec3(0,1,0), 1.1);
	hitable *world = new hitable_list(list,4);
	camera cam;
	for (int j=ny-1; j>=0; j--) {
		for (int i=0; i<nx; i++) {
			vec3 col(0,0,0);
			for (int s=0; s<ns; s++) {
				float u = float(i + drand48())/float(nx);
				float v = float(j + drand48())/float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r, world, 0);
			}
			col = col / float(ns);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}
