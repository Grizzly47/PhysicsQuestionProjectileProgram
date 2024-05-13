#include <iostream>
#include <iomanip>
#include <cmath>
#include <SFML/Graphics.hpp>

#define M_PI 3.14159265358979323846

// Adjust numbers to be displayed rounded to a certain number of decimals
double adjustNumbers(double _dbl) {
    if (_dbl < 0.01 && _dbl > -0.01)
        return 0.0;
    else
        return std::round(_dbl * std::pow(10, 2)) / std::pow(10, 2);
}

// Function to perform the quadratic formula
double quadraticSolution(double a, double b, double c) {
    std::cout << "A: " << a << " B: " << b << " C: " << c << std::endl;
    double quatradicPlus = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    double quatradicNeg = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    if (quatradicPlus > quatradicNeg)
        return quatradicPlus;
    else
        return quatradicNeg;
}

int main()
{
    // Inputs
    double alpha, beta, gamma, v0, initialHeight;

    // Input values
    std::cout << "Please input Alpha (degrees)" << std::endl;
    std::cin >> alpha;
    std::cout << "Please input Beta (degrees)" << std::endl;
    std::cin >> beta;
    std::cout << "Please input Gamma (degrees)" << std::endl;
    std::cin >> gamma;
    std::cout << "Please input Initial velocity" << std::endl;
    std::cin >> v0;
    std::cout << "Please input initial height" << std::endl;
    std::cin >> initialHeight;

    // Convert angles from degrees to radians
    alpha = alpha * M_PI / 180.0;
    beta = beta * M_PI / 180.0;
    gamma = gamma * M_PI / 180.0;

    // Calculate initial velocities in x, y, and z directions
    double vx = v0 * cos(alpha) * cos(beta); // vx = v0 * cos(alpha) * cos(beta)
    double vy = v0 * sin(alpha);             // vy = v0 * sin(alpha)
    double vz = v0 * cos(alpha) * sin(beta); // vz = v0 * cos(alpha) * sin(beta)

    // Assuming no air resistance
    double g = -9.81;

    // Calculate time of flight
    double time = quadraticSolution(g / 2, vy, initialHeight);
    double height = 0;

    // Set screen size
    const int screenWidth = 400;
    const int screenHeight = 400;
    const int scale = 20;
    const int trajectoryPoints = 10;

    // Create SFML windows for top-down and side views
    sf::RenderWindow topDownView(sf::VideoMode(screenWidth, screenHeight), "Top-Down View (X/Z)");
    sf::RenderWindow sideView(sf::VideoMode(screenWidth, screenHeight), "Side View (X/Y)");

    // Main loop
    while (topDownView.isOpen() && sideView.isOpen())
    {
        // Handle events
        sf::Event event;
        while (topDownView.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                topDownView.close();
        }
        while (sideView.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                sideView.close();
        }

        // Clear the windows
        topDownView.clear(sf::Color::White);
        sideView.clear(sf::Color::White);

        // Draw the trajectory in the top-down view
        sf::VertexArray trajectoryTop(sf::LineStrip, trajectoryPoints);
        for (int i = 0; i < trajectoryPoints; ++i)
        {
            double t = (double)i / (double)trajectoryPoints * time;
            double x = vx * t;
            double z = vz * t;
            trajectoryTop[i].position = sf::Vector2f(x * scale, screenHeight - z * scale); // Start from bottom left corner
            trajectoryTop[i].color = sf::Color::Black;
        }
        topDownView.draw(trajectoryTop);

        // Draw the trajectory in the side view
        sf::VertexArray trajectorySide(sf::LineStrip, trajectoryPoints);
        for (int i = 0; i < trajectoryPoints; ++i)
        {
            double currentTime = (double)i / (double)trajectoryPoints * time;
            double z = vz * currentTime;
            double y = vy * currentTime + 0.5 * g * currentTime * currentTime + initialHeight;
            trajectorySide[i].position = sf::Vector2f(z * scale, screenHeight - y * scale); // Start from bottom left corner
            trajectorySide[i].color = sf::Color::Black;
        }
        sideView.draw(trajectorySide);

        // Display the windows
        topDownView.display();
        sideView.display();
    }

    // Calculate maximum height
    height = (pow(vy, 2) + (2 * g) * initialHeight) / (-2 * g);

    // Output results
    std::cout << std::endl;
    std::cout << "Time of flight: " << adjustNumbers(time) << " seconds" << std::endl;
    std::cout << "Maximum height: " << adjustNumbers(height) << " meters" << std::endl;
    std::cout << "X of landing position: " << adjustNumbers(vx * time / 2) << " meters" << std::endl;
    std::cout << "Z of landing position: " << adjustNumbers(vz * time / 2) << " meters" << std::endl;

    return 0;
}