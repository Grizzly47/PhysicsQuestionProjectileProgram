#include <iostream>
#include <iomanip>
#include <cmath>
#include <SFML/Graphics.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Adjust numbers to be displayed rounded to a certain number of decimals
double adjustNumbers(double _dbl) {
    if (_dbl < 0.01 && _dbl > -0.01)
        return 0.0;
    else
        return std::round(_dbl * std::pow(10, 2)) / std::pow(10, 2);
}

// Function to perform the quadratic formula
double quadraticSolution(double a, double b, double c) {
    double quadraticPlus = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    double quadraticNeg = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    if (quadraticPlus > quadraticNeg)
        return quadraticPlus;
    else
        return quadraticNeg;
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
    double vx = v0 * cos(alpha);
    double vy = v0 * cos(beta);
    double vz = v0 * cos(gamma);

    // Assuming no air resistance
    double g = -9.81;

    // Calculate time of flight
    double time = quadraticSolution(g / 2, vy, -initialHeight);

    // Create SFML windows for top-down and side views
    sf::RenderWindow topDownView(sf::VideoMode(800, 400), "Top-Down View");
    sf::RenderWindow sideView(sf::VideoMode(800, 400), "Side View");



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
        sf::VertexArray trajectoryTop(sf::LineStrip);
        for (int t = 0; t <= 100; ++t)
        {
            double x = t; // Simplified trajectory for debugging
            double y = t * t / 100.0; // Simplified trajectory for debugging
            trajectoryTop.append(sf::Vector2f(static_cast<float>(x), static_cast<float>(-y))); // Invert y-axis
        }
        topDownView.draw(trajectoryTop);

        // Draw the trajectory in the side view
        sf::VertexArray trajectorySide(sf::LineStrip, static_cast<std::size_t>(time * 10)); // Adjust as needed
        for (int t = 0; t <= time * 10 && t < trajectorySide.getVertexCount(); ++t)
        {
            double z = vz * t / 10; // Scale down by 10 for better visualization
            double y = initialHeight + vy * t / 10 + 0.5 * g * pow(t / 10, 2); // Scale down by 10 for better visualization
            trajectorySide[t].position = sf::Vector2f(static_cast<float>(z), static_cast<float>(-y)); // Invert y-axis
            trajectorySide[t].color = sf::Color::Red;
        }
        sideView.draw(trajectorySide);

        // Display the windows
        topDownView.display();
        sideView.display();
    }

    return 0;
}

/*

// Adjust numbers to be displayed rounded to a certain number of decimals
double adjustNumbers(double _dbl) {
    if (_dbl < 0.01 && _dbl > -0.01)
        return 0.0;
    else
        return std::round(_dbl * std::pow(10, 2)) / std::pow(10, 2);
}

// Funciton to perform the quadratic fomula
double quadraticSolution(double a, double b, double c) {
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

    // Results
    double time = 0, height = 0;

    // Input values (std::cin or whatever)
    std::cout << "Please input Alpha (degrees)" << std::endl;
    std::cin >> alpha;
    std::cout << "Please input Beta (degrees)" << std::endl;
    std::cin >> beta;
    std::cout << "Please input Gamma (degrees)" << std::endl;
    std::cin >> gamma;
    std::cout << "Please input Initial velocity" << std::endl;
    std::cin >> v0;

    // Difference between point A and B
    std::cout << "Please input inital height" << std::endl;
    std::cin >> initialHeight;

    // Convert angles from degrees to radians (cmath needs radians)
    alpha = alpha * M_PI / 180.0;
    beta = beta * M_PI / 180.0;
    gamma = gamma * M_PI / 180.0;

    // Calculate initial velocities in x, y, and z directions
    double vx = v0 * cos(alpha);
    double vy = v0 * cos(beta); // cos beta
    double vz = v0 * cos(gamma);

    // Assuming no air resistance
    double g = -9.81;

    // Calculate time of flight
    time = quadraticSolution(g / 2, vy, -initialHeight);

    // Calculate maximum height
    height = (pow(vy, 2) + (2 * g) * initialHeight) / (-2 * g);

    // Output results
    std::cout << std::endl;
    std::cout << "Time of flight: " << adjustNumbers(time) << " seconds" << std::endl;
    std::cout << "Maximum height: " << adjustNumbers(height) << " meters" << std::endl;
    std::cout << "X of landing position: " << adjustNumbers(vx * time) << " meters" << std::endl;
    std::cout << "Z of landing position: " << adjustNumbers(vz * time) << " meters" << std::endl;

    return 0;
}
*/