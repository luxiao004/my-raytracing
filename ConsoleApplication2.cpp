#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"
#include "bvh.h"
#include "camera.h"
#include "material.h"
#include <ctime>
#include <cstdlib>
using namespace std;

color ray_color(const ray& r,const hittable& world,int depth) {
    if (depth <= 0)return color(0, 0, 0);
    hit_record rec;
	//====背景色部分====
    if (!world.hit(r, 0.001, INFINITY, rec)) {
        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.1, 0.1, 1.0);   
    }
    //====材质反射散射自发光部分====
	color emitted = rec.mat_ptr ? rec.mat_ptr->emitted() : color(0, 0, 0);
    ray scattered;
    color attenuation;
    if (!rec.mat_ptr ||! rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
        return emitted;
    }
	return emitted + attenuation * ray_color(scattered, world, depth - 1);
}
int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
	//=========材质定义=====
	auto material_ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.0);
	auto light_material = make_shared<diffuse_light>(color(7, 7, 7));
	//=========图像参数=====
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    // 相机位置 & 朝向
    point3 lookfrom(5.5, 1.5, 3.0);   // 相机放在斜侧上方
    point3 lookat(0.5,0.5,0.5);   
    vec3 vup(0, 1, 0);
    double vfov = 30;
    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio);
	//=========场景构建=====
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));
    world.add(make_shared<sphere>(point3(0.8, 0, -1), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, material_left));
	world.add(make_shared<sphere>(point3(5, 5, -2), 1.0, light_material));
    //====输出 PPM=====
    std::ofstream outfile("C:\\Users\\lx\\Pictures\\Saved Pictures\\rtlx03.ppm");
    outfile <<"P3\n" << image_width << ' ' << image_height << "\n255\n";
	// 逐行逐列采样
    for (int j = image_height - 1;j >= 0;--j) {
        std::cerr<< "\rScanlines remaining:" << j << ' ' << std::flush;
        for (int i = 0;i < image_width;++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0;s < samples_per_pixel;++s) {
                auto u = (i+random_double())/ (image_width - 1);
                auto v = (j+random_double())/ (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color+= ray_color(r,world,5);
            }
			auto scale = 1.0 / samples_per_pixel;
			double r = sqrt(scale * pixel_color.x());
			double g = sqrt(scale * pixel_color.y());
			double b = sqrt(scale * pixel_color.z());
            
            int ir = static_cast<int>(256 * clamp(r,0.0,0.999));
            int ig = static_cast<int>(256 * clamp(g, 0.0, 0.999));
            int ib = static_cast<int>(256 * clamp(b, 0.0, 0.999));

            outfile << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    outfile.close();
    std::cerr << "\nDone.Image saved to rtlx02.ppm\n";
    std::cout << "按Enter键退出";
    std::cin.get();
    return 0;
}

