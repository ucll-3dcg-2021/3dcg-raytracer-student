#include "raytracers/ray-tracer-v4.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

imaging::Color raytracer::raytracers::_private_::RayTracerV4::process_light_ray(const Scene& scene, const MaterialProperties& props, const Hit& hit, const math::Ray& ray, LightRay& lightRay) const
{
	Hit rayhit;

	//hit between light and scene
	if (scene.root->find_first_positive_hit(lightRay.ray, &rayhit))
	{
		double t = rayhit.t;

		//hit between Light and hit Position -> shade
		if (0 <= t && t <= 0.99)
		{
			return colors::black();
		}
	}
	return RayTracerV3::process_light_ray(scene, props, hit, ray, lightRay);
}

raytracer::RayTracer raytracer::raytracers::v4()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV4>());
}
