#include <SFML/Graphics.hpp>
#include <iostream>
#include "Planet.cpp"
#define AA_LEVEL 8U
#define G 6.67*std::pow(10, -11)
//#define EARTH_MASS 50000000
//#define SUN_MASS 1665271300000
#define EARTH_MASS 5.972f * pow(10, 24)
#define SUN_MASS 1.989f * pow(10, 30)
// Mass of Earth in real life is 5.972 * 10^24 kg
// Mass of the Sun in real life is 1.989 * 10^30 kg

int main(int argc, char const** argv) {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Planets", 7U, sf::ContextSettings(0U, 0U, AA_LEVEL));

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
    sf::Clock frameClock;
    
    sf::View camera(sf::Vector2f(0, 0), sf::Vector2f(400000000, -400000000));
    window.setView(camera);

    sf::Text notToScale("Not to scale.", font, 12U);
    notToScale.setPosition(5, window.getSize().y - 17);

    // Radius of the planets isn't accurate, but it shouldn't matter
    Planet sun(6963400*2, SUN_MASS);
    sun.setOrigin(sun.getRadius(), sun.getRadius());
    sun.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    sun.setFillColor(sf::Color::Yellow);

    Planet earth(1274200*2, EARTH_MASS);
    earth.setOrigin(earth.getRadius(), earth.getRadius());
    earth.setPosition(sun.getPosition().x - 150000000, 0);
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
            // Down arrow: Add force down
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
                earth.addForce(sf::Vector2f(20, 0));
            }
            // Up arrow: Add force up
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
                earth.addForce(sf::Vector2f(-20, 0));
            }
            // Right arrow: Add force right
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
                earth.addForce(sf::Vector2f(0, 20));
            }
            // Left arrow: Add force left
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
                earth.addForce(sf::Vector2f(0, -20));
            }
        }
        // Calculate framerate
        float framerate = 1.f / frameClock.getElapsedTime().asSeconds();
        fpsCounter.setString(sf::String(std::to_string(framerate)));
        frameClock.restart();

        // Check collision with the window
        // Check collision with the right side
        if (earth.getPosition().x >= window.getSize().x - earth.getRadius()) {
            earth.multForce(sf::Vector2f(-1, 1));
        }
        // Check collision with the bottom
        if (earth.getPosition().y >= window.getSize().y - earth.getRadius()) {
            earth.multForce(sf::Vector2f(1, -1));
        }
        // Check collision with the left side
        if (earth.getPosition().x <= earth.getRadius()) {
            earth.multForce(sf::Vector2f(-1, 1));
        }
        // Check collision with the top side
        if (earth.getPosition().y <= earth.getRadius()) {
            earth.multForce(sf::Vector2f(1, -1));
        }

        // The actual formula needs a square root over everything, however
        // when we use r in calculating the gravitational pull of the planets,
        // we square it, so it cancels out.
        float r = pow(earth.getPosition().x - sun.getPosition().x, 2) + pow(earth.getPosition().y - sun.getPosition().y, 2);

        float earthAngle = atan2(
            earth.getPosition().y - sun.getPosition().y,
            earth.getPosition().x - sun.getPosition().x
            );
        float sunAngle = atan2(
            sun.getPosition().y - earth.getPosition().y,
            sun.getPosition().x - earth.getPosition().x
            );
        float F = G * earth.getMass() * sun.getMass() / r;

        earth.setForce(sf::Vector2f(
            (earth.getForce().x + F * cos(sunAngle)) / framerate,
            (earth.getForce().y + F * sin(sunAngle)) / framerate
            ));
        //earth.addForce(sf::Vector2f(0, 30));
        std::cout << "F is " << F << "\nForce on x is " << earth.getForce().x << "\nForce on y is " << earth.getForce().y << std::endl;

        // earth.addForce(sf::Vector2f(0, 9.81f * 1/framerate)); // Gravity on earth; Used for learning
        earth.move(sf::Vector2f(earth.getForce().x * 1/framerate, earth.getForce().y * 1/framerate));

        // Clear screen
        window.clear(sf::Color(20, 20, 20));

        window.draw(fpsCounter);
        window.draw(notToScale);

        window.draw(earth);
        window.draw(sun);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}