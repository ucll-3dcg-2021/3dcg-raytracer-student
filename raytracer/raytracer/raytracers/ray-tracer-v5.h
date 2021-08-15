#pragma once

#include "raytracers/ray-tracer-v4.h"
#include "raytracers/ray-tracer.h"
#include <memory>


namespace raytracer
{
    namespace raytracers
    {
        namespace _private_
        {
            class RayTracerV5 : public RayTracerV4
            {
            public:
                TraceResult trace(const Scene&, const math::Ray&) const override;
                
                // new trace overload with weight
                virtual TraceResult trace(const Scene&, const math::Ray&, const double) const;
                
                imaging::Color compute_reflection(const Scene& scene, const MaterialProperties& props, const Hit& hit, const math::Ray& ray, double weight) const;
            };
        }
        RayTracer v5();
    }
}