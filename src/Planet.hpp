#ifndef PI
#define PI 3.141592654f
#endif
#include <SFML/Graphics.hpp>
#include <cmath>

class Planet : public sf::CircleShape {
public:
    explicit Planet();

    double getRadius();

    void setRadius(double radius);

    void setMass(double mass);

    double getMass();

    void setPointCount(int pointCount);

    virtual std::size_t getPointCount() const;

    virtual sf::Vector2f getPoint(std::size_t index) const;

private:
    double m_radius;
    long double m_mass;
    std::size_t m_pointCount;
};