#include <SFML/Graphics.hpp>
#include <iostream>
#include "Planet.hpp"
#define AA_LEVEL 8U
#define EARTH_MASS 5.972f * pow(10, 24)
#define SUN_MASS 1.989f * pow(10, 30)
// Mass of Earth in real life is 5.972 * 10^24 kg
// Mass of the Sun in real life is 1.989 * 10^30 kg

bool planetCollision(Planet planet1, Planet planet2) {
    float r = sqrt(pow(planet1.getPosition().x - planet2.getPosition().x, 2) + pow(planet1.getPosition().y - planet2.getPosition().y, 2));
    return r <= planet1.getRadius() + planet2.getRadius();
}

int main(int argc, char const** argv) {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Planets", 7U, sf::ContextSettings(0U, 0U, AA_LEVEL));

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile("src/icon.png")) { return EXIT_FAILURE; }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Set the font
    sf::Font font;
    if (!font.loadFromFile("src/sansation.ttf")) { return EXIT_FAILURE; }
    
    sf::View camera(sf::Vector2f(0, 0), sf::Vector2f(400000000, -400000000));
    window.setView(camera);

    sf::Vector2f lineStartPos;
    sf::Vector2f lineEndPos;
    sf::RectangleShape line(sf::Vector2f(100000000, 500000));
    line.setOrigin(0, line.getSize().y / 2);
    line.setFillColor(sf::Color(255, 255, 255, 0));
    bool shouldDrawLine = false;

    std::vector<Planet*> planets;

    Planet sun(0, 0);
    planets.push_back(&sun);
    sun.setRadius(13926800);
    sun.setMass(SUN_MASS);
    sun.setOrigin(sun.getRadius(), sun.getRadius());
    sun.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    sun.setFillColor(sf::Color::Yellow);
    sun.indestructible = true;

    // Inverse speed of time. A value of 2 will make time pass 2x slower
    float timeScale = 2000000;

    sf::Clock timePassed;
    sf::Clock frameClock;

    // Start the game loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            // Left click: Initialize the line and start drawing it
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                shouldDrawLine = true;
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                lineStartPos = window.mapPixelToCoords(pixelPos);
            }
            // Release left click: Make line invisible and save the mouse position
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                line.setFillColor(sf::Color(255, 255, 255, 0));
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                lineEndPos = window.mapPixelToCoords(pixelPos);
                shouldDrawLine = false;
                double angle = atan2(
                    lineStartPos.y - lineEndPos.y,
                    lineStartPos.x - lineEndPos.x
                );
                float distance = sqrt(pow(lineStartPos.x - lineEndPos.x, 2) + pow(lineStartPos.y - lineEndPos.y, 2));
                // Variable to control how much or how little you have to pull
                // the mouse to launch the planet
                int sensitivity = 100;
                sf::Vector2f velocity(distance * cos(angle) / sensitivity, distance * sin(angle) / sensitivity);

                planets.push_back(new Planet(velocity));
                size_t size = planets.size() - 1;
                planets[size]->setRadius(2548400);
                planets[size]->setMass(EARTH_MASS);
                planets[size]->setOrigin(planets[size]->getRadius(), planets[size]->getRadius());
                planets[size]->setPosition(lineStartPos);
                planets[size]->setFillColor(sf::Color::Cyan);
                planets[size]->addVelocity(velocity);
            }
        }

        // Calculate time difference between last frame and current frame
        float deltaTime = 1.f / frameClock.getElapsedTime().asSeconds();
        frameClock.restart();
        deltaTime = deltaTime / timeScale;
        
        if (timePassed.getElapsedTime().asSeconds() > 0.25f) {
            sf::String windowTitle((std::string("Planet simulation [").append(std::to_string((int)(deltaTime * timeScale)))).append(" FPS]"));
            window.setTitle(windowTitle);
            timePassed.restart();
        }

        if (planets.size() > 1) {
            for (size_t i = 0; i < planets.size(); i++) {
                planets[i]->calculateVelocity(planets, deltaTime);
                planets[i]->move(planets[i]->getVelocity() * deltaTime);
            }
        }

        // Line thing for launching Earth
        if (shouldDrawLine) {
            // Initialize the line
            line.setFillColor(sf::Color::White);
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            line.setPosition(worldPos);

            // Rotate the line towards the original point
            double angle = atan2(
                lineStartPos.y - worldPos.y,
                lineStartPos.x - worldPos.x
            );
            line.setRotation(angle * 180 / PI);

            // Resize the line according to its correct length
            float length = sqrt(pow(lineStartPos.x - worldPos.x, 2) + pow(lineStartPos.y - worldPos.y, 2));
            line.setSize(sf::Vector2f(length, line.getSize().y));
        }

        // Detect collision between planets
        for (size_t i = 0; i < planets.size(); i++)
            for (size_t j = 0; j < planets.size(); j++)
                if (planetCollision(*planets[i], *planets[j]) && i != j) {
                    int removeToNotGetAllocationError = 0;
                    if (!planets[i]->indestructible) {
                        planets.erase(planets.begin() + i);
                        removeToNotGetAllocationError++;
                    }
                    if (!planets[j]->indestructible)
                        planets.erase(planets.begin() + j - removeToNotGetAllocationError);
                }
                
        // Clear screen
        window.clear(sf::Color(20, 20, 20));

        for (size_t i = 0; i < planets.size(); i++) {
            window.draw(*planets[i]);
        }

        window.draw(line);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}