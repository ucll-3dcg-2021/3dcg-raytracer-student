#include "samplers/random-sampler.h"
#include <random>

using namespace math;
using namespace raytracer;


namespace
{
    class RandomSampler : public raytracer::samplers::_private_::SamplerImplementation
    {
    public:
        int m_sample_count;


        RandomSampler(const int& sample_count)
        {
            m_sample_count = sample_count;
        }

        std::vector<Point2D> sample(const math::Rectangle2D& rectangle) const override
        {
            std::vector<Point2D> result;

            std::random_device rd;
            std::mt19937 gen(rd());

            std::uniform_real_distribution<> dis(0, 1);
            for (int n = 0; n < m_sample_count; ++n) {
                result.push_back(rectangle.from_relative(Point2D(dis(gen), dis(gen))));
            }

            return result;
        }
    };
}

Sampler raytracer::samplers::random(const int& m_sample_count)
{
    return Sampler(std::make_shared<RandomSampler>(m_sample_count));
}