#include <SFML/Graphics.hpp>
#include <iostream>
#include "Planet.cpp"
#define AA_LEVEL 8U
#define G 6.67*std::pow(10, -11)

int main(int argc, char const** argv) {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Planets", 7U, sf::ContextSettings(0U, 0U, AA_LEVEL));

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile("src/icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Set the font
    sf::Font font;
    if (!font.loadFromFile("src/sansation.ttf")) {
        return EXIT_FAILURE;
    }
    // And add the placeholder text for the framerate
    sf::Text fpsCounter("Placeholder text. If this doesn't change, the app is frozen", font, 16U);
    fpsCounter.setPosition(0,0);
    sf::Clock clock;

    sf::Text notToScale("Not to scale.", font, 12U);
    notToScale.setPosition(5, window.getSize().y - 17);

    Planet sun(30.f, 30, 1.989*std::pow(10,30));
    sun.setOrigin(sun.getRadius(), sun.getRadius());
    sun.setPosition(400, 300);
    sun.setFillColor(sf::Color::Yellow);

    Planet earth(5.f, 30, 5.972*std::pow(10,24));
    earth.setOrigin(earth.getRadius(), earth.getRadius());
    earth.setPosition(400, 150);
    earth.setFillColor(sf::Color::Cyan);
    

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
        }

        // Clear screen
        window.clear(sf::Color(28, 28, 28));

        // Calculate and show framerate
        float framerate = 1.f / clock.getElapsedTime().asSeconds();
        fpsCounter.setString(sf::String(std::to_string(framerate)));
        clock.restart();
        window.draw(fpsCounter);
        window.draw(notToScale);

        window.draw(earth);
        window.draw(sun);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}