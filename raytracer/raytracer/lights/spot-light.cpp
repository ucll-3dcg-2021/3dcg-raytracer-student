#include "lights/spot-light.h"

using namespace math;
using namespace raytracer;

namespace
{
    class SpotLight : public raytracer::lights::_private_::PointLightImplementation
    {
    public:
        SpotLight(const math::Point3D& position, const Vector3D& direction, const Angle& angle, const imaging::Color& color)
            : PointLightImplementation(position), m_direction(direction), m_angle(angle), m_color(color) { }

    protected:
        LightRay cast_lightray_to(const math::Point3D& position) const override
        {
            math::Ray ray(m_position, position);

            Vector3D cos = (position - m_position).normalized();

            if (cos.dot(m_direction) >= math::cos(m_angle / 2))
            {
                return LightRay(ray, m_color);

            }
            return LightRay(ray, imaging::colors::black());
        }

    private:
        imaging::Color m_color;
        Vector3D m_direction;
        Angle m_angle;
    };
}

LightSource raytracer::lights::spot(const math::Point3D& position, const math::Vector3D& direction, const math::Angle& angle, const imaging::Color& color)
{
    return LightSource();
}
