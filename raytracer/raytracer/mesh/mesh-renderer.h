#pragma once
#include "primitives/primitive.h"

using namespace math;

namespace raytracer
{
	namespace primitives
	{
		Primitive mesh_renderer(const std::string& file);
	}
}