#include "raytracers/ray-tracer-v5.h"

using namespace imaging;
using namespace math;
using namespace raytracer;


TraceResult raytracer::raytracers::_private_::RayTracerV5::trace(const Scene& scene, const Ray& ray) const
{
	return trace(scene, ray, 1.0);
}

//trace overload with weight
TraceResult raytracer::raytracers::_private_::RayTracerV5::trace(const Scene& scene, const math::Ray& ray, const double weight) const
{
	Hit hit;

	if (scene.root->find_first_positive_hit(ray, &hit))
	{
		Color result = colors::black();

		//check if weight is still worth computing ( weight < 0.01 no )
		if (weight >= 0.01)
		{
			//normal ray computing
			Color hit_color = hit.material->at(hit.local_position).ambient;
			result += hit_color;

			result += process_lights(scene, hit.material->at(hit.local_position), hit, ray);

			//add reflection computing
			result += compute_reflection(scene, hit.material->at(hit.local_position), hit, ray, weight);
		}

		unsigned group_id = hit.group_id;

		double t = hit.t;

		return TraceResult(result, group_id, ray, t);
	}
	else
	{
		return TraceResult::no_hit(ray);
	}
}

imaging::Color raytracer::raytracers::_private_::RayTracerV5::compute_reflection(const Scene& scene, const MaterialProperties& props, const Hit& hit, const math::Ray& ray, double weight) const
{
	Color result = colors::black();

	//check if material is reflectif
	if (props.reflectivity > 0)
	{
		Vector3D direction_ray = (hit.position - ray.origin).normalized();

		Vector3D direction_reflection = direction_ray.reflect_by(hit.normal);

		//scew origin to avoid getting hit Position as reflection
		Point3D new_Origin = ray.at(hit.t) + 0.00000001 * direction_reflection;

		Ray reflection = Ray(new_Origin, direction_reflection);

		result += (props.reflectivity * trace(scene, reflection, props.reflectivity * weight).color);
	}

	return result;
}


raytracer::RayTracer raytracer::raytracers::v5()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV5>());
}
