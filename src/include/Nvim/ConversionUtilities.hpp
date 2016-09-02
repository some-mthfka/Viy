#ifndef CONVERSION_UTILITIES_HPP
#define CONVERSION_UTILITIES_HPP

#include <SFML/Graphics/Color.hpp>
#include <nvim/DataHolder.hpp>       //nvim::CellAttribute

//nvim sends colors as integers and there is a need to 
//convert those values to sf::Color
sf::Color                   toColor(uint32_t value);

//Nvim sends information about text and background attributes to use.
//This function does the conversion 
nvim::CellAttribute          toCellAttribute(int32_t id, uint32_t value);

//Program receives unicode text from Python and needs to convert it
//sf::String, which needs UTF32.
//Makes the program depend on boost.
std::basic_string<char32_t> UTF8ToUTF32(const char *str, uint32_t length);

#endif
