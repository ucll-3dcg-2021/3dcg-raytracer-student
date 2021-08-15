#pragma once

#include "samplers/sampler.h"


namespace raytracer
{
    namespace samplers
    {
        /// <summary>
        /// Creates a random sampler that always picks sample_count random points of the given rectangle.
        /// </summary>
        Sampler random(const int& sample_count);
    }
}
