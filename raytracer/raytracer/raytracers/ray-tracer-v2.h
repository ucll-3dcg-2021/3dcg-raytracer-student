#pragma once

#include "raytracers/ray-tracer.h"
#include <memory>


namespace raytracer
{
    namespace raytracers
    {
        namespace _private_
        {
            class RayTracerV2 : public RayTracerImplementation
            {
            public:
                imaging::Color process_lights(const Scene&,
                    const MaterialProperties&, const Hit&,
                    const math::Ray&) const;
                imaging::Color process_light_source(const Scene&,
                    const MaterialProperties&, const Hit&,
                    const math::Ray&, LightSource) const;
                virtual imaging::Color process_light_ray(const Scene&,
                    const MaterialProperties&, const Hit&,
                    const math::Ray&, LightRay&) const;
                imaging::Color compute_diffuse(const MaterialProperties&,
                    const Hit&, const math::Ray&, LightRay&) const;
                TraceResult trace(const Scene&, const math::Ray&) const override;
            };
        }

        /// <summary>
        /// Creates simplest ray tracer.
        /// </summary>
        RayTracer v2();
    }
}