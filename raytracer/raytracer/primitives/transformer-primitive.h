#pragma once

#include "primitives/primitive.h"
#include "math/transformation3d.h"
#include <memory>

namespace raytracer
{
    namespace primitives
    {
        Primitive transform(const math::Transformation3D&, Primitive);
        Primitive translate(const math::Vector3D&, Primitive);
        Primitive scale(double sx, double sy, double sz, Primitive transformee);
        Primitive rotate_around_x(const math::Angle, const Primitive);
        Primitive rotate_around_y(const math::Angle, const Primitive);
        Primitive rotate_around_z(const math::Angle, const Primitive);
    }
}