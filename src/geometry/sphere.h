#pragma once

#include "core/geometry.h"
using namespace glm;

class Sphere : public Geometry {
public:
    Sphere() {}
    Sphere(dvec4 center, double radius, vec3 color) : Geometry(color), center(center), radius(radius) {}
    bool intersect(const Ray &ray, double &tmin, Shade &shade) const;
    ~Sphere() {}
private:
    dvec4 center;
    double radius;
};

