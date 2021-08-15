#include "math/transformation3d.h"
#include "math/transformation-matrices.h"

using namespace math;


Transformation3D math::transformations::translation(const Vector3D& v)
{
    Matrix4x4 tm = transformation_matrices::translation(v);
    Matrix4x4 itm = transformation_matrices::translation(-v);

    return Transformation3D(tm, itm);
}

Transformation3D math::transformations::rotate_around_x(const Angle& x)
{
    Matrix4x4 tm = math::transformation_matrices::rotation_around_x(x);
    Matrix4x4 itm = math::transformation_matrices::rotation_around_x(-x);

    return Transformation3D(tm, itm);
}

Transformation3D math::transformations::rotate_around_y(const Angle& x)
{
    Matrix4x4 tm = math::transformation_matrices::rotation_around_y(x);
    Matrix4x4 itm = math::transformation_matrices::rotation_around_y(-x);

    return Transformation3D(tm, itm);
}

Transformation3D math::transformations::rotate_around_z(const Angle& x)
{
    Matrix4x4 tm = math::transformation_matrices::rotation_around_z(x);
    Matrix4x4 itm = math::transformation_matrices::rotation_around_z(-x);

    return Transformation3D(tm, itm);
}

Transformation3D math::transformations::scale(double sx, double sy, double sz) {
    Matrix4x4 tm = transformation_matrices::scaling(sx, sy, sz);
    Matrix4x4 itm = transformation_matrices::scaling(-sx, -sy, -sz);

    return Transformation3D(tm, itm);
}
