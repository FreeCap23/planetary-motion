#include <SFML/Graphics.hpp>
#include <cmath>

#define PI 3.141592654f
// Mass is measured in kg
class Planet : public sf::CircleShape {
public :
    explicit Planet(const double& radius = 0.f, const long double& mass = 10) : m_radius(radius), m_mass(mass) {
        m_force = sf::Vector2f(0, 0);
        m_pointCount = 30;
        m_isAlive = true;
        update();
    }

    void setMass(const long double& mass) {
        m_mass = mass;
    }

    long double getMass() {
        return m_mass;
    }

    void addForce(const sf::Vector2f force) {
        m_force += force;
    }

    void setForce(const sf::Vector2f force) {
        m_force = force;
    }

    void multForce(const sf::Vector2f force) {
        m_force.x = m_force.x * force.x;
        m_force.y = m_force.y * force.y;
    }

    sf::Vector2f getForce() {
        return m_force;
    }

    void setRadius(const double& radius) {
        m_radius = radius;
        update();
    }

    void destroy() {
        m_isAlive = false;
    }

    bool isAlive() {
        return m_isAlive;
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
    bool m_isAlive;
    sf::Vector2f m_force;
    long double m_mass;
    std::size_t m_pointCount;
    double m_radius;
};