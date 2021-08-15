#include "primitives/triangle-primitive.h"
#include "math/interval.h"

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;

namespace {
	class TriangleImplementation : public raytracer::primitives::_private_::PrimitiveImplementation {
	public:
		Vector3D normal_vector_on_plane;
		Point3D p1;
		Point3D p2;
		Point3D p3;

		//triangle constructor, taking 3 points representing vertices
		TriangleImplementation(const Point3D& p1, const Point3D& p2, const Point3D& p3) : p1(p1), p2(p2), p3(p3)
		{
			//compute normal vector (remains constant)
			this->normal_vector_on_plane = (p2 - p1).cross(p3 - p1).normalized();
		}

		bool find_first_positive_hit(const Ray& ray, Hit* hit) const override {
			//compute hit
			double t = (p1 - ray.origin).dot(normal_vector_on_plane) / ray.direction.dot(normal_vector_on_plane);
			if (t < 0 || t >= hit->t) return false;

			//compute hit position: H
			Point3D H = ray.origin + (ray.direction * t);

			//check position of H
			if ((p2 - p1).cross(H - p1).dot(normal_vector_on_plane) < 0) return false;
			if ((p3 - p2).cross(H - p2).dot(normal_vector_on_plane) < 0) return false;
			if ((p1 - p3).cross(H - p3).dot(normal_vector_on_plane) < 0) return false;

			initialize_hit(hit, ray, t);

			//return true if all previous checks are true
			return true;
		}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override
		{
			std::vector<std::shared_ptr<Hit>> hits;

			double t = (p1 - ray.origin).dot(normal_vector_on_plane) / ray.direction.dot(normal_vector_on_plane);
			
			Point3D H = ray.origin + (ray.direction * t);

			if ((p2 - p1).cross(H - p1).dot(normal_vector_on_plane) < 0) return hits;
			if ((p3 - p2).cross(H - p2).dot(normal_vector_on_plane) < 0) return hits;
			if ((p1 - p3).cross(H - p3).dot(normal_vector_on_plane) < 0) return hits;

			auto hit = std::make_shared<Hit>();
			initialize_hit(hit.get(), ray, t);
			hits.push_back(hit);

			return hits;
		}

		Box bounding_box() const override
		{
			auto x_min = std::min({ p1.x(), p2.x(), p3.x() });
			auto y_min = std::min({ p1.y(), p2.y(), p3.y() });
			auto z_min = std::min({ p1.z(), p2.z(), p3.z() });

			auto x_max = std::max({ p1.x(), p2.x(), p3.x() });
			auto y_max = std::max({ p1.y(), p2.y(), p3.y() });
			auto z_max = std::max({ p1.z(), p2.z(), p3.z() });

			return Box(
				interval(x_min, x_max),
				interval(y_min, y_max),
				interval(z_min, z_max)
			);
		}

	protected:
		void initialize_hit(Hit* hit, const Ray& ray, double t) const
		{
			hit->t = t;
			hit->position = ray.at(hit->t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(hit->position.x(), hit->position.y());
			hit->normal = ray.direction.dot(normal_vector_on_plane) < 0 ? normal_vector_on_plane : -normal_vector_on_plane;
		}
	};
}

Primitive raytracer::primitives::triangle(const math::Point3D& p1, const math::Point3D& p2, const math::Point3D& p3) {
	return Primitive(std::make_shared < TriangleImplementation>(p1, p2, p3));
}


