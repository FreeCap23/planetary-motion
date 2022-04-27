#include "Planet.hpp"

Planet::Planet(sf::Vector2f initialVelocity) {
    m_velocity = initialVelocity;
    m_pointCount = 30;
    indestructible = false;
    update();
}

Planet::Planet(float initialVelocityX, float initialVelocityY) {
    m_velocity.x = initialVelocityX;
    m_velocity.y = initialVelocityY;
    m_pointCount = 30;
    update();
}

void Planet::calculateVelocity(std::vector<Planet*> planets, float deltaTime) {
    for (int i = 0; i < planets.size(); i++) {
        if (planets[i] != this) {
            double square_r = pow(planets[i]->getPosition().x - this->getPosition().x, 2) + pow(planets[i]->getPosition().y - this->getPosition().y, 2);
            double angle = atan2(
                planets[i]->getPosition().y - this->getPosition().y,
                planets[i]->getPosition().x - this->getPosition().x
            );
            double F = UNIVERSAL_CONSTANT * planets[i]->getMass() * m_mass / square_r;
            sf::Vector2f acceleration(F * cos(angle) / m_mass, F * sin(angle) / m_mass);
            m_velocity += acceleration * deltaTime;
        }
    }
}

void Planet::addVelocity(sf::Vector2f force) {
    sf::Vector2f acceleration = sf::Vector2f(force.x, force.y);
    m_velocity += acceleration;
}

sf::Vector2f Planet::getVelocity() {
    return m_velocity;
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