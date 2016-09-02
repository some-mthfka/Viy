#include <Debug.hpp>

void endTimer(sf::Clock &timer)
{
    auto elapsed = timer.getElapsedTime().asMilliseconds();
    if (elapsed)
        std::cout << "Time elapsed = " << elapsed << std::endl;
    timer.restart();
}
