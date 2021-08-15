#pragma once

#include "raytracers/ray-tracer-v2.h"
#include <memory>
#include <cstdlib>


namespace raytracer
{
	namespace raytracers
	{
		namespace _private_
		{
			//subclasses v2
			class RayTracerV3 : public RayTracerV2
			{
			public:
				imaging::Color process_light_ray(const Scene&,
					const MaterialProperties&, const Hit&,
					const math::Ray&, LightRay&) const override;
				//compute reflection of light source
				imaging::Color compute_specular(const MaterialProperties&, const Hit&, const math::Ray&, const LightRay&) const;
			};
		}

		RayTracer v3();
	}
}