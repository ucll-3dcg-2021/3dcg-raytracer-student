#include "bounding-box-accelerator.h"

using namespace raytracer;
using namespace math;

namespace
{
	class BoundingBoxAccelerator : public primitives::_private_::PrimitiveImplementation
	{
	public:
		const Primitive primitive;
		const Box bounding_box_child;

		BoundingBoxAccelerator(const Primitive& primitive) :primitive(primitive), bounding_box_child(primitive->bounding_box()) {};

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override
		{
			if (!bounding_box_child.is_hit_positively_by(ray))
			{
				return std::vector<std::shared_ptr<Hit>>();
			}
			return primitive->find_all_hits(ray);
		}

		bool find_first_positive_hit(const Ray& ray, Hit* hit) const override
		{
			if (!bounding_box_child.is_hit_positively_by(ray))
			{
				return false;
			}
			return primitive->find_first_positive_hit(ray, hit);
		}

		Box bounding_box() const override
		{
			return bounding_box_child;
		}
	};
}

Primitive primitives::bounding_box_accelerator(const Primitive& primitive)
{
	return Primitive(std::make_shared<BoundingBoxAccelerator>(primitive));
}
