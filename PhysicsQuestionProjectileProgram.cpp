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
    double discriminant = pow(b, 2) - 4 * a * c;
    if (discriminant < 0) {
        std::cerr << "Error: Discriminant is negative. No real solutions." << std::endl;
        return -1; // or handle the error appropriately
    }
    double quatradicPlus = (-b + sqrt(discriminant)) / (2 * a);
    double quatradicNeg = (-b - sqrt(discriminant)) / (2 * a);
    return (quatradicPlus > quatradicNeg) ? quatradicPlus : quatradicNeg;
}

static void drawGrid(sf::RenderWindow& window, int gridSpacing, int screenWidth, int screenHeight) {
    sf::Color LightGray(200, 200, 200);
    sf::VertexArray gridLines(sf::Lines);
    // Draw vertical lines
    for (int x = 0; x <= screenWidth; x += gridSpacing) {
        gridLines.append(sf::Vertex(sf::Vector2f(x, 0), LightGray));
        gridLines.append(sf::Vertex(sf::Vector2f(x, screenHeight), LightGray));
    }
    // Draw horizontal lines
    for (int y = 0; y <= screenHeight; y += gridSpacing) {
        gridLines.append(sf::Vertex(sf::Vector2f(0, y), LightGray));
        gridLines.append(sf::Vertex(sf::Vector2f(screenWidth, y), LightGray));
    }
    window.draw(gridLines);
}

int main()
{
    // Inputs
    double alpha, beta, gamma, v0, initialHeight, barrelLength;

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
    std::cout << "Please input barrel length" << std::endl;
    std::cin >> barrelLength;

    // Convert angles from degrees to radians
    alpha = alpha * M_PI / 180.0;
    beta = beta * M_PI / 180.0;
    gamma = gamma * M_PI / 180.0;

    // Adjust initial positions based on the barrel length
    double initialXOffset = barrelLength * cos(alpha) * cos(beta);
    double initialZOffset = barrelLength * cos(alpha) * sin(beta);

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
    const int trajectoryPoints = 100;
    const int gridSpacing = 20; // Set grid spacing

    // Create SFML windows for top-down and side views
    sf::RenderWindow topDownView(sf::VideoMode(screenWidth, screenHeight), "Top-Down View (X/Z)");
    sf::RenderWindow sideView(sf::VideoMode(screenWidth, screenHeight), "Side View (X/Y)");
    sf::RenderWindow zySideView(sf::VideoMode(screenWidth, screenHeight), "Side View (Z/Y)");

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
        while (zySideView.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                zySideView.close();
        }

        // Clear the windows
        topDownView.clear(sf::Color::White);
        sideView.clear(sf::Color::White);
        zySideView.clear(sf::Color::White);

        // Draw the grid in the top-down view
        drawGrid(topDownView, gridSpacing, screenWidth, screenHeight);

        // Draw the grid in the side view
        drawGrid(sideView, gridSpacing, screenWidth, screenHeight);

        // Draw ZY grid
        drawGrid(zySideView, gridSpacing, screenWidth, screenHeight);

        // Draw the trajectory in the top-down view
        sf::VertexArray trajectoryTop(sf::LineStrip, trajectoryPoints);
        for (int i = 0; i < trajectoryPoints; ++i)
        {
            double t = (double)i / (double)trajectoryPoints * time;
            double x = vx * t;
            double z = vz * t;
            trajectoryTop[i].position = sf::Vector2f((x + initialXOffset) * scale, screenHeight - (z + initialZOffset) * scale);
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
            trajectorySide[i].position = sf::Vector2f((z + initialZOffset) * scale, screenHeight - y * scale);
            trajectorySide[i].color = sf::Color::Black;
        }
        sideView.draw(trajectorySide);

        // Draw the trajectory in the Z/Y side view
        sf::VertexArray trajectoryZY(sf::LineStrip, trajectoryPoints);
        for (int i = 0; i < trajectoryPoints; ++i)
        {
            double currentTime = (double)i / (double)(trajectoryPoints - 1) * time;
            double z = vz * currentTime;
            double y = vy * currentTime + 0.5 * g * currentTime * currentTime + initialHeight;
            trajectoryZY[i].position = sf::Vector2f((z + initialZOffset) * scale, screenHeight - y * scale);
            trajectoryZY[i].color = sf::Color::Black;
        }
        zySideView.draw(trajectoryZY);

        // Display the windows
        topDownView.display();
        sideView.display();
        zySideView.display();
    }

    // Calculate maximum height
    height = (vy * vy) / (2 * -g) + initialHeight;

    // Output results
    std::cout << std::endl;
    std::cout << "Time of flight: " << adjustNumbers(time) << " seconds" << std::endl;
    std::cout << "Maximum height: " << adjustNumbers(height) << " meters" << std::endl;
    std::cout << "X of landing position: " << adjustNumbers(vx * time) << " meters" << std::endl;
    std::cout << "Z of landing position: " << adjustNumbers(vz * time) << " meters" << std::endl;

    return 0;
}