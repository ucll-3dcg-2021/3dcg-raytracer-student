#include <fstream>
#include <vector>
#include <stack>
#include <sstream>
#include "primitives/mesh-primitive.h"
#include "primitives/primitives.h"
#include <math/box.h>
#include <math.h>  
#include <map>
#include <iomanip>

void raytracer::primitives::mesh_primitive()
{
	struct TrianglePoints
	{
		math::Point3D p1;
		math::Point3D p2;
		math::Point3D p3;

		bool operator==(TrianglePoints a) {
			if (a.p1 == p1 && a.p2 == p2 && a.p3 == p3)
				return true;
			else
				return false;
		}
		std::string toString() {
			std::ostringstream streamObj;
			streamObj << std::fixed;
			streamObj << std::setprecision(20);
			streamObj << p1.x() << " " << p1.y() << " " << p1.z() << " " << p2.x() << " " << p2.y() << " " << p2.z() << " " << p3.x() << " " << p3.y() << " " << p3.z();
			return streamObj.str();
		}
	};

	std::ifstream input("C:/Users/rafae/Documents/dragon.mesh");

	int n_vertices;
	input >> n_vertices;
	std::vector<math::Point3D> vertices;
	for (auto i = 0; i < n_vertices; i++)
	{
		double x, y, z;
		input >> x >> y >> z;
		math::Point3D point = math::Point3D(x, y, z);
		vertices.push_back(point);
	}

	int n_triangles;
	input >> n_triangles;

	std::vector<Primitive> triangles;
	std::vector<TrianglePoints> trianglePoints;
	for (auto i = 0; i < n_triangles; i++)
	{
		int i1, i2, i3;
		input >> i1 >> i2 >> i3;
		TrianglePoints a;
		a.p1 = vertices[i1];
		a.p2 = vertices[i2];
		a.p3 = vertices[i3];
		Primitive triangle = raytracer::primitives::triangle(a.p1, a.p2, a.p3);
		triangles.push_back(triangle);
		trianglePoints.push_back(a);
	}

	Primitive all_triangles = raytracer::primitives::make_union(triangles);

	math::Box all_bounding_box = all_triangles->bounding_box(); // this is the biggest possible bounding box

	std::vector<math::Box> boxes;

	boxes.push_back(all_bounding_box);

	std::vector<math::Box> top_boxes;

	int n_split = 17;

	for (auto i = 0; i < n_split; i++)
	{
		for (auto j = 0; j < pow(2, i); j++)
		{
			math::Box big = boxes[boxes.size() - 1];
			math::Box split1 = math::Box::empty();
			math::Box split2 = math::Box::empty();

			if (i % 3 == 0) {
				auto x1 = math::interval<double>(big.x().lower, big.x().center());
				auto x2 = math::interval<double>(big.x().center(), big.x().upper);
				split1 = math::Box::Box(x1, big.y(), big.z());
				split2 = math::Box::Box(x2, big.y(), big.z());
			}
			else if (i % 3 == 1) {
				auto y1 = math::interval<double>(big.y().lower, big.y().center());
				auto y2 = math::interval<double>(big.y().center(), big.y().upper);
				split1 = math::Box::Box(big.x(), y1, big.z());
				split2 = math::Box::Box(big.x(), y2, big.z());
			}
			else {
				auto z1 = math::interval<double>(big.z().lower, big.z().center());
				auto z2 = math::interval<double>(big.z().center(), big.z().upper);
				split1 = math::Box::Box(big.x(), big.y(), z1);
				split2 = math::Box::Box(big.x(), big.y(), z2);
			}


			top_boxes.push_back(split1);
			top_boxes.push_back(split2);

			boxes.pop_back();
		}

		for each (math::Box box in top_boxes)
		{
			boxes.push_back(box);
		}
		top_boxes.clear();
	}


	std::map<int, std::vector<TrianglePoints>> box_map;
	for (auto i = 0; i < boxes.size(); i++)
	{
		std::vector<TrianglePoints> temp;
		box_map[i] = temp;
	}
	for (auto i = 0; i < triangles.size(); i++)
	{
		for (auto j = 0; j < pow(2, n_split); j++)
		{
			if (boxes[j].contains(trianglePoints[i].p1) || boxes[j].contains(trianglePoints[i].p2) || boxes[j].contains(trianglePoints[i].p3)) {
				box_map.find(j)->second.push_back(trianglePoints[i]);
				break;
			}
		}
	}

	std::ofstream output("C:/Users/rafae/Documents/ray-tracer-quidoussemousse/mesh/output.txt");

	for (auto i = boxes.size() - 1; i > 0; i--)
	{
		for each (TrianglePoints var in box_map[i])
		{
			output << "t " << var.toString() << "\n";
		}
		output << "box " << box_map[i].size() << "\n";
	}

	for each (TrianglePoints var in box_map[0])
	{
		output << "t " << var.toString() << "\n";
	}
	output << "box " << box_map[0].size() << "\n";

	output << "end";
}


