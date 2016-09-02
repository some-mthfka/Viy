#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <Nvim/InitData.hpp>

#include <SFML/System/String.hpp>
#include <SFML/Graphics/Font.hpp>

#include <sstream>
#include <string>

namespace sf
{
	class Sprite;
	class Text;
}

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string toString(const T& value);

//template <typename T>
//std::basic_string<sf::Uint32> toUtf32String(const T& value);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);

int getLineSpacing(const sf::Font &font, int fontSize);
int getCharWidth(const sf::Font &font, int fontSize);
void constructVideoMode(nvim::InitData &initData);
void Stealth();

#include <Utility.inl>
#endif // UTILITY_HPP
