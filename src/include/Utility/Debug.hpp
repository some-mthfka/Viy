#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>

#define TRACE(ARG) std::cout << #ARG << " = " << ARG << std::endl;
#define LOG(TEXT) std::cout << #TEXT << std::endl;

#include <SFML/System/Clock.hpp>

void endTimer(sf::Clock &timer);

#endif //DEBUG_HPP
