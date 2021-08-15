#include "samplers/multijittered-sampler.h"
#include "math/rasterizer.h"
#include <random>
#include <numeric>

using namespace math;
using namespace raytracer;

namespace
{
	class MultijitteredSampler : public samplers::_private_::SamplerImplementation
	{
	public:
		int n;

		MultijitteredSampler(const int& n)
		{
			this->n = n;
		}

		std::vector<Point2D> sample(const Rectangle2D& rectangle) const override
		{
			std::vector<Point2D> result;

			std::vector<int> random_point_index(n * n);
			std::iota(random_point_index.begin(), random_point_index.end(), 0);
			std::random_shuffle(random_point_index.begin(), random_point_index.end());

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(0, 1);

			auto rasterizer = Rasterizer(rectangle, n, n);

			for (auto i = 0; i < n * n; i++)
			{
				auto sub_rec = rasterizer[Position2D(i / n, i % n)];
				auto sub_rasterizer = Rasterizer(sub_rec, n, n);
				auto sub_sub_rec = sub_rasterizer[Position2D(random_point_index[i] / n, random_point_index[i] % n)];
				result.push_back(sub_sub_rec.from_relative(Point2D(dis(gen), dis(gen))));
			}
			return result;
		}
	};
}

Sampler samplers::multijittered(const int& n)
{
	return Sampler(std::make_shared<MultijitteredSampler>(n));
}