#ifndef PI
#define PI 3.141592654f
#define UNIVERSAL_CONSTANT 6.67*std::pow(10, -11)
#endif
#include <SFML/Graphics.hpp>
#include <cmath>

class Planet : public sf::CircleShape {
public:
    Planet(sf::Vector2f initialVelocity);

    Planet(float initialVelocityX, float initialVelocityY);

    void calculateVelocity(std::vector<Planet*> planets, float deltaTime);

    void addVelocity(sf::Vector2f force);

    sf::Vector2f getVelocity();

    double getRadius();

    void setRadius(double radius);

    void setMass(double mass);

    double getMass();

    void setPointCount(int pointCount);

    virtual std::size_t getPointCount() const;

    virtual sf::Vector2f getPoint(std::size_t index) const;

public:
    bool indestructible;

private:
    double m_radius;
    long double m_mass;
    sf::Vector2f m_velocity;
    std::size_t m_pointCount;
};