#include <fstream>
#include <iostream>
#include <stack>
#include "primitives/primitives.h"
#include <istream>
#include <sstream>
#include "easylogging++.h"
#include "primitives/bounding-box-accelerator.h"

using namespace std;
using namespace raytracer;
using namespace math;
using namespace primitives;

Primitive primitives::mesh_renderer(const std::string& file)
{
	ifstream input(file);
	std::vector<Primitive> primitives;
	Primitive final_box;

	string sort;
	double x1, y1, z1, x2, y2, z2, x3, y3, z3;

	while (true)
	{
		input >> sort;
		if (sort == "t")
		{
			input >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
			const Point3D& p1 = Point3D(x1, y1, z1);
			const Point3D& p2 = Point3D(x2, y2, z2);
			const Point3D& p3 = Point3D(x3, y3, z3);
			Primitive t = triangle(p1, p2, p3);
			primitives.push_back(t);
		}
		else if (sort == "box")
		{
			int x = 0;
			input >> x;
			if (x > 0) {
				std::vector<Primitive> triangles;
				for (int i = 0; i < x; i++)
				{
					Primitive driehoekOFdoos = primitives[primitives.size() - 1];
					primitives.pop_back();
					triangles.push_back(driehoekOFdoos);

				}
				Primitive boundenDoos =
					bounding_box_accelerator(make_union(triangles));
				primitives.push_back(boundenDoos);
			}
		}
		else if (sort == "end")
		{
			final_box =
				bounding_box_accelerator(make_union(primitives));
			break;
		}
	}
	return final_box;
}