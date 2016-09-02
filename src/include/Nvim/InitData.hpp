#ifndef INIT_DATA_HPP
#define INIT_DATA_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>

//Data read from ini file is kept in InitData

namespace nvim
{
    struct InitData
    {
        sf::Font
            font;

        sf::VideoMode
            videoMode;

        std::string 
            pathToFont,
            fileToOpen;

        int 
            rows = -1, cols = -1,
            fontSize = -1,
            //used in case of misalignment of background and text
            xTextOffset = 0, yTextOffset = 0;

        bool
            showFPS = false;
    };
}

#endif
