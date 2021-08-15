#include "samplers/jittered-sampler.h"
#include "math/rasterizer.h"
#include <random>

using namespace math;
using namespace raytracer;

namespace
{
	class JitteredSampler : public samplers::_private_::SamplerImplementation
	{
	public:
		int n;
		int m;

		JitteredSampler(const int& n, const int& m)
		{
			this->n = n;
			this->m = m;
		}

		std::vector<Point2D> sample(const Rectangle2D& rectangle) const override
		{
			std::vector<Point2D> result;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(0, 1);
			auto rasterizer = Rasterizer(rectangle, n, m);
			for (auto i = 0; i < n; i++)
			{
				for (auto j = 0; j < m; j++)
				{
					auto raster_rec = rasterizer[Position2D(i, j)];
					result.push_back(raster_rec.from_relative(Point2D(dis(gen), dis(gen))));
				}
			}
			return result;
		}
	};
}

Sampler samplers::jittered(const int& n, const int& m)
{
	return Sampler(std::make_shared<JitteredSampler>(n, m));
}