#include "world.h"
#include "geometry/primitive/sphere.h"
#include "geometry/primitive/plane.h"
#include "geometry/primitive/box.h"
#include "geometry/primitive/disk.h"
#include "geometry/primitive/rectangle.h"
#include "geometry/instance.h"
#include "geometry/compound/grid.h"
#include "geometry/model/model.h"
#include "material/matte.h"
#include "material/phong.h"
#include "material/emissive.h"
#include "material/reflective.h"
#include "material/glossyreflector.h"
#include "material/transparent.h"
#include "light/ambientoccluder.h"
#include "light/arealight.h"
#include "tracer/globaltracer.h"
#include "camera/pinhole.h"
#include "camera/thinlens.h"
#include "sampler/regular.h"
#include "sampler/multijittered.h"

void World::build() {
    /* Viewplane */
    vp.horRes = 400;
    vp.vertRes = 400;
    vp.pixelSize = 0.005;
    vp.maxDepth = 5;
    vp.globalIllum = true;
    vp.setSamples(25, 4);
    vp.gamma = 1.0;

    /* Materials */
    auto plastic1P = new Phong(vec3(0.25, 0.72, 0.96), 0.2, 0.4, 0.3);
    auto plastic2P = new Phong(vec3(0.44, 0.24, 0.61), 0.4, 0.6, 0.1);
    plastic2P->setSpecularExponent(8.0f);
    auto plastic3P = new Matte(vec3(1.0), 0.4, 0.6);
    auto emi1P = new Emissive(vec3(1.0, 0.82, 0.59), 25.0);
    auto plastic4P = new Phong(vec3(0.14, 0.47, 0.8), 0.3, 0.6, 0.1);
    auto bronzeP = new GlossyReflector(vec3(0.89, 0.36, 0.14), 0.2, 0.2, 0.1, 0.6);
    auto silver1P = new Reflective(vec3(1.0), 0.1, 0.2, 0.1, 0.8);
    auto silver2P = new GlossyReflector(vec3(1.0), 0.2, 0.2, 0.1, 0.7);
    silver2P->setGlossyReflectionExponent(100.0f);
    auto plastic5P = new Matte(vec3(0.9, 0.2, 0.13), 0.3, 0.7);
    auto plastic6P = new Matte(vec3(0.33, 0.78, 0.25), 0.3, 0.7);
    auto mirror1P = new Reflective(vec3(1.0), 0.2, 0.2, 0.3, 0.6);
    auto glass1P = new Transparent(vec3(1.0), 0.2, 0.1, 0.1, 0.3, 0.5, 1.5);

    /* Geometry Objects */
    auto sphere1P = new Sphere(plastic1P);
    sphere1P->setParams(dvec3(-1.00, 0.80, 0), .80);
    auto sphere2P = new Sphere(glass1P);
    sphere2P->setParams(dvec3(0, 0.40, -1.00), .4001);
    auto plane1P = new Plane(plastic3P);
    plane1P->setParams(dvec3(0, 1, 0), dvec3(0.0, 0.0, 0.0));
    auto disk1P = new Disk(emi1P);
    disk1P->setParams(dvec3(0, 1.99, -1.00), dvec3(0, -1, 0), 0.30);
    disk1P->setSampler(new MultiJittered(256, 2));
    disk1P->toggleShadowCast(false);
    auto modelP = new Model("resources/bunny.obj", glass1P);
    auto inst1P = new Instance(modelP);
    inst1P->scale(dvec3(0.50))->translate(dvec3(0.10, 0, -1.20));
    auto rect1P = new Rectangle(silver2P); // back
    rect1P->setParams(dvec3(-1.00, 2.00, -2.00), dvec3(0, -2.00, 0), dvec3(2.00, 0, 0));
    auto rect2P = new Rectangle(plastic3P); // up
    rect2P->setParams(dvec3(-1.00, 2.00, -2.00), dvec3(2.00, 0, 0), dvec3(0, 0, 2.00));
    auto rect3P = new Rectangle(plastic5P); // left
    rect3P->setParams(dvec3(-1.00, 2.00, -2.00), dvec3(0, 0, 2.00), dvec3(0, -2.00, 0));
    auto rect4P = new Rectangle(mirror1P); // bottom
    rect4P->setParams(dvec3(-1.00, 0, -2.00), dvec3(0, 0, 2.00), dvec3(2.00, 0, 0));
    auto rect5P = new Rectangle(plastic6P); // right
    rect5P->setParams(dvec3(1.00, 0, -2.00), dvec3(0, 0, 2.00), dvec3(0, 2.00, 0));

    addObject(disk1P);
    addObject(sphere2P);
    addObject(rect1P);
    addObject(rect2P);
    addObject(rect3P);
    addObject(rect4P);
    addObject(rect5P);

    /* Lights */
    bgColor = vec3();
    AmbientOccluder *occluderP = new AmbientOccluder(vec3(1.0, 0.82, 0.59), 1.0, 0.2);
    setAmbient(occluderP);

    auto area1P = new AreaLight(disk1P);
    area1P->toggleShadowCast(true);
    addLight(area1P);

    /* Camera & Tracer */
    tracerP = new GlobalTracer(this);
    PinHole *cam = new PinHole(dvec3(0, 1.00, 2.10), dvec3(0, 1.00, 0), 2.00);
    cameraP = cam;
    _pixels = new unsigned char[vp.horRes * vp.vertRes * vp.numChannels];
    finished = 0;
}