#include "raytracers/ray-tracer-v3.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

//override v2 process_light_ray
imaging::Color raytracer::raytracers::_private_::RayTracerV3::process_light_ray(const Scene& scene, const MaterialProperties& props, const Hit& hit, const math::Ray& ray, LightRay& lightRay) const
{
    //v2 base
    Color result = RayTracerV2::process_light_ray(scene, props, hit, ray, lightRay);

    // add specular highlights
    result += compute_specular(props, hit, ray, lightRay);

    return result;
}

//compute reflection of light source
imaging::Color raytracer::raytracers::_private_::RayTracerV3::compute_specular(const MaterialProperties& props, const Hit& hit, const math::Ray& ray, const LightRay& lightRay) const
{
    Color result = colors::black();

    Point3D L = lightRay.ray.origin;
    Color Cl = lightRay.color;
    Point3D P = hit.position;
    Color Cp = props.specular;
    double e = props.specular_exponent;
    Point3D E = ray.origin;
    math::Vector3D vectorN = hit.normal;

    //direction incoming Light
    math::Vector3D incoming_light = (P - L).normalized();

    //unit vector from hit Position to Eye
    math::Vector3D unit_vector = (E - P).normalized();

    //reflected direction
    math::Vector3D reflected_direction = incoming_light.reflect_by(vectorN);

    //compute cosine to determine amount of photons reaching Eye from Light
    double cos = unit_vector.dot(reflected_direction);

    //cosine > 0 -> compute photons
    if (cos > 0)
    {
        result += Cl * Cp * pow(cos, e);
    }
    return result;
}

raytracer::RayTracer raytracer::raytracers::v3()
{
    return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV3>());
}
