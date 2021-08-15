#include "raytracers/ray-tracer-v2.h"

using namespace imaging;
using namespace math;
using namespace raytracer;


TraceResult raytracer::raytracers::_private_::RayTracerV2::trace(const Scene& scene, const Ray& ray) const
{
	Hit hit;

	// Ask the scene for the first positive hit, i.e. the closest hit in front of the eye
	// If there's a hit, find_first_positive_hit returns true and updates the hit object with information about the hit
	if (scene.root->find_first_positive_hit(ray, &hit))
	{
		// no light bounces -> return "shade"
		Color result = colors::black();
		// There's been a hit
		// Fill in TraceResult object with information about the trace

		// raytracer returns color of material at hit point (colors aren't uniform)
		Color hit_color = hit.material->at(hit.local_position).ambient;

		result += hit_color;

		result += process_lights(scene, hit.material->at(hit.local_position), hit, ray);

		// The hit object contains the group id, just copy it (group ids are important for edge detection)
		unsigned group_id = hit.group_id;

		// The t-value indicates where the ray/scene intersection took place.
		// You can use ray.at(t) to find the xyz-coordinates in space.
		double t = hit.t;

		// Group all this data into a TraceResult object.
		return TraceResult(hit_color, group_id, ray, t);
	}
	else
	{
		// The ray missed all objects in the scene
		// Return a TraceResult object representing "no hit found"
		// which is basically the same as returning black
		return TraceResult::no_hit(ray);
	}
}
imaging::Color raytracer::raytracers::_private_::RayTracerV2::process_lights(const Scene& scene, const MaterialProperties& materialProperties, const Hit& hit, const math::Ray& ray) const
{
	Color result = colors::black();

	//go over each light source to see if rays hit
	for each (LightSource lightsource in scene.light_sources)
	{
		result += process_light_source(scene, materialProperties, hit, ray, lightsource);
	}
	return result;
}

imaging::Color raytracer::raytracers::_private_::RayTracerV2::process_light_source(const Scene& scene, const MaterialProperties& props, const Hit& hit, const math::Ray& ray, LightSource lightSource) const
{
	Color result = colors::black();

	// go over each ray in a light source to check if it hits
	for each (LightRay lightRay in lightSource->lightrays_to(hit.position)) {
		result += process_light_ray(scene, props, hit, ray, lightRay);
	}

	return result;
}

//
imaging::Color raytracer::raytracers::_private_::RayTracerV2::process_light_ray(const Scene& scene, const MaterialProperties& props, const Hit& hit, const math::Ray& ray, LightRay& lightRay) const
{
	Color result = colors::black();

	result += compute_diffuse(props, hit, ray, lightRay);

	return result;
}

//compute diffuse lighting mathematically
imaging::Color raytracer::raytracers::_private_::RayTracerV2::compute_diffuse(const MaterialProperties& props, const Hit& hit, const math::Ray& ray, LightRay& lightRay) const
{
	Color result = colors::black();
	Color Cl = lightRay.color;
	math::Point3D L = lightRay.ray.origin;
	math::Point3D P = hit.position;
	Color Cs = props.diffuse;
	math::Vector3D vectorN = hit.normal;

	double cos = (L - P).normalized().dot(vectorN);

	if (cos > 0)
	{
		result += cos * Cl * Cs;
	}
	return result;
}

raytracer::RayTracer raytracer::raytracers::v2()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV2>());
}