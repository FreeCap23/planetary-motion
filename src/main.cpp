#include <SFML/Graphics.hpp>
#include <iostream>
#include "Planet.hpp"
#define AA_LEVEL 8U
#define G 6.67*std::pow(10, -11)
//#define EARTH_MASS 50000000
//#define SUN_MASS 1665271300000
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
    // And add the placeholder text for the framerate
    sf::Text fpsCounter("Placeholder text. If this doesn't change, the app is frozen", font, 16U);
    fpsCounter.setPosition(0,0);
    sf::Clock frameClock;
    
    sf::View camera(sf::Vector2f(0, 0), sf::Vector2f(400000000, -400000000));
    window.setView(camera);

    sf::Text notToScale("Not to scale.", font, 12U);
    notToScale.setPosition(5, window.getSize().y - 17);

    sf::Vector2f lineStartPos;
    sf::Vector2f lineEndPos;
    sf::RectangleShape line(sf::Vector2f(100000000, 500000));
    line.setOrigin(0, line.getSize().y / 2);
    line.setFillColor(sf::Color(255, 255, 255, 0));
    bool shouldDrawLine = false;

    Planet* planets[2];

    Planet sun;
    planets[0] = &sun;
    sun.setRadius(13926800);
    sun.setMass(SUN_MASS);
    sun.setOrigin(sun.getRadius(), sun.getRadius());
    sun.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    sun.setFillColor(sf::Color::Yellow);

    Planet earth;
    planets[1] = &earth;
    earth.setRadius(2548400);
    earth.setMass(EARTH_MASS);
    earth.setOrigin(earth.getRadius(), earth.getRadius());
    earth.setPosition(sun.getPosition().x - 150000000, 30000000);
    earth.setFillColor(sf::Color::Cyan);

    float timeScale = 1500;
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
            }
        }
        
        // Calculate framerate
        float framerate = 1.f / frameClock.getElapsedTime().asSeconds();
        fpsCounter.setString(sf::String(std::to_string(framerate)));
        frameClock.restart();
        framerate = framerate / timeScale;

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

        if (planetCollision(earth, sun))
            earth.setPosition(sun.getPosition().x - 150000000, 30000000);

        // Clear screen
        window.clear(sf::Color(20, 20, 20));

        window.draw(fpsCounter);
        window.draw(notToScale);

        for (int i = 0; i < 2; i++) {
            window.draw(*planets[i]);
        }

        window.draw(line);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}