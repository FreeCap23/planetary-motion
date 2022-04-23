#include "Planet.hpp"

Planet::Planet() {
    m_pointCount = 30;
    m_radius = 10.f;
    update();
}

double Planet::getRadius() {
    return m_radius;
}

void Planet::setRadius(double radius) {
    m_radius = radius;
    update();
}

void Planet::setMass(double mass) {
    m_mass = mass;
}

double Planet::getMass() {
    return m_mass;
}

void Planet::setPointCount(int pointCount) {
    m_pointCount = pointCount;
    update();
}

std::size_t Planet::getPointCount() const {
    return m_pointCount;
}

sf::Vector2f Planet::getPoint(std::size_t index) const {
    float angle = index * 2 * PI / getPointCount();
    float x = std::cos(angle) * m_radius;
    float y = std::sin(angle) * m_radius;

    return sf::Vector2f(m_radius + x, m_radius + y);
};