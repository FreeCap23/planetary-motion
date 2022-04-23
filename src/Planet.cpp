#include <SFML/Graphics.hpp>
#include <cmath>

#define PI 3.141592654f
// Mass is measured in kg
class Planet : public sf::CircleShape {
public :
    explicit Planet(const double radius = 0.f, const long double mass = 10) : m_radius(radius), m_mass(mass) {
        m_pointCount = 30;
        m_isAlive = true;
        update();
    }

    void setPointCount(const int pointCount) {
        m_pointCount = pointCount;
        update();
    }

    const double& getRadius() const {
        return m_radius;
    }

    virtual std::size_t getPointCount() const {
        return m_pointCount;
    }

    virtual sf::Vector2f getPoint(std::size_t index) const {
        float angle = index * 2 * PI / getPointCount();
        float x = std::cos(angle) * m_radius;
        float y = std::sin(angle) * m_radius;

        return sf::Vector2f(m_radius + x, m_radius + y);
    }

private :
    double m_radius;
    long double m_mass;
    std::size_t m_pointCount;
};