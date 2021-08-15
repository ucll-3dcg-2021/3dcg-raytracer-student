#include "samplers/nrooks-sampler.h"
#include "math/rasterizer.h"
#include <random>
#include <numeric>

using namespace math;
using namespace raytracer;

namespace
{
	class NRooksSampler : public samplers::_private_::SamplerImplementation
	{
	public:
		int n;

		NRooksSampler(const int& n)
		{
			this->n = n;
		}

		std::vector<Point2D> sample(const Rectangle2D& rectangle) const override
		{
			std::vector<Point2D> result;

			std::vector<int> random_row(n);
			std::iota(random_row.begin(), random_row.end(), 0);
			std::shuffle(random_row.begin(), random_row.end(), std::mt19937{ std::random_device{}() });

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(0, 1);

			auto rasterizer = Rasterizer(rectangle, n, n);
			for (auto i = 0; i < n; i++)
			{
				auto raster_rec = rasterizer[Position2D(i, random_row[i])];
				result.push_back(raster_rec.from_relative(Point2D(dis(gen), dis(gen))));
			}
			return result;
		}
	};
}

Sampler samplers::nrooks(const int& n)
{
	return Sampler(std::make_shared<NRooksSampler>(n));
}