#include "ray-tracer-v6.h"
#include "ray-tracer-v5.h"
#include "raytracers/ray-tracers.h"

using namespace imaging;
using namespace math;
using namespace raytracer;


TraceResult raytracer::raytracers::_private_::RayTracerV6::trace(const Scene& scene, const math::Ray& ray, const double weight) const
{
	Hit hit;

	if (scene.root->find_first_positive_hit(ray, &hit))
	{
		Color result = colors::black();
		if (weight >= 0.01)
		{
			Color hit_color = hit.material->at(hit.local_position).ambient;
			result += hit_color;
			result += process_lights(scene, hit.material->at(hit.local_position), hit, ray);

			result += compute_reflection(scene, hit.material->at(hit.local_position), hit, ray, weight);

			//add refraction
			result += compute_refraction(scene, hit.material->at(hit.local_position), ray, hit, weight);
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

imaging::Color raytracer::raytracers::_private_::RayTracerV6::compute_refraction(const Scene& scene, const MaterialProperties& props, const math::Ray& ray, const Hit& hit, double weight) const
{
	//check if material is transparant
	if (props.transparency > 0)
	{
		Ray refracted_ray = calc_refracted_ray(scene, ray, hit, 1.0, props.refractive_index);

		//check if ray is refracted back to origin
		if (refracted_ray == ray) return colors::black();

		Hit exit_hit;
		if (!scene.root->find_first_positive_hit(refracted_ray, &exit_hit))
		{
			return colors::black();
		}
		else {
			Ray exit_ray = calc_refracted_ray(scene, refracted_ray, exit_hit, props.refractive_index, 1.0);
			return trace(scene, exit_ray, weight * props.transparency).color * props.transparency;
		}
	}
	return colors::black();
}

math::Ray raytracer::raytracers::_private_::RayTracerV6::calc_refracted_ray(const Scene& scene, const math::Ray& ray, const Hit& hit, double n1, double n2) const
{
	Vector3D incoming_direction = (hit.position - ray.origin).normalized();

	Vector3D outgoing_x = (n1 / n2) * (incoming_direction - (incoming_direction.dot(hit.normal)) * hit.normal);

	//check for total internal reflection
	if ((1 - outgoing_x.norm_sqr()) < 0)
	{
		return ray;
	}
	else {
		//calculate outgoing direction of ray
		Vector3D outgoing_y = (0 - sqrt(1 - outgoing_x.norm_sqr())) * hit.normal;
		Vector3D outgoing_direction = outgoing_x + outgoing_y;
		Point3D outgoing_origin = ray.at(hit.t) + (0.00000001 * outgoing_direction);
		Ray refracted_ray = Ray(outgoing_origin, outgoing_direction);
		return refracted_ray;
	}
}

RayTracer raytracer::raytracers::v6()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV6>());
}