#include "sphere.h"

bool Sphere::intersect(const Ray &ray, double &tmin, Shade &shade) const {
    double t;
    dvec3 omc = dvec3(ray.origin - center);
    double a = dot(ray.direction, ray.direction);
    double b = 2.0 * dot(omc, dvec3(ray.direction));
    double c = dot(omc, omc) - radius * radius;
    double disc = b * b - 4.0 * a * c;

    if (disc < 0) return false;
    else {
        double e = sqrt(disc);
        double denom = 2.0 * a;
        t = (-b - e) / denom; // smaller root
        if (t > EPSILON) {
            tmin = t;
            shade.normal = (omc + t * dvec3(ray.direction)) / radius;
            shade.hitPoint = ray.origin + t * ray.direction;
            return true;
        }
        t = (-b + e) / denom;
        if (t > EPSILON) {
            tmin = t;
            shade.normal = (omc + t * dvec3(ray.direction)) / radius;
            shade.hitPoint = ray.origin + t * ray.direction;
            return true;
        }
    }
    return false;
}
