#include "samplers/stratified-sampler.h"
#include "math/rasterizer.h"

using namespace math;
using namespace raytracer;

namespace
{
	class StratifiedSampler : public samplers::_private_::SamplerImplementation
	{
	public:
		int n;
		int m;

		StratifiedSampler(const int& n, const int& m)
		{
			this->n = n;
			this->m = m;
		}

		std::vector<Point2D> sample(const Rectangle2D& rectangle) const override
		{
			std::vector<Point2D> result;
			auto rasterizer = Rasterizer(rectangle, n, m);
			for (auto i = 0; i < n; i++)
			{
				for (auto j = 0; j < m; j++)
				{
					auto raster_rec = rasterizer[Position2D(i, j)];
					result.push_back(raster_rec.center());
				}
			}
			return result;
		}
	};
}

Sampler samplers::stratified(const int& n, const int& m)
{
	return Sampler(std::make_shared<StratifiedSampler>(n, m));
}