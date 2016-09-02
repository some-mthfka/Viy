#include <Utility.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>

#include <Debug.hpp>

void centerOrigin(sf::Sprite& sprite)
{
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(sf::Text& text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

int getLineSpacing(const sf::Font &font, int fontSize)
{
    return font.getLineSpacing(fontSize);
}

int getCharWidth(const sf::Font &font, int fontSize)
{
    return font.getGlyph('A', fontSize, false).advance;
}

void constructVideoMode(nvim::InitData &initData)
{
    initData.videoMode.width  = initData.cols * getCharWidth(initData.font, 
                                                             initData.fontSize); 
    initData.videoMode.height = initData.rows * getLineSpacing(initData.font,
                                                               initData.fontSize);

    //in case the requested rows or cols cannot fit into the screen
    if (initData.videoMode.width > sf::VideoMode::getDesktopMode().width)
    {
        initData.videoMode.width = sf::VideoMode::getDesktopMode().width;
        initData.cols = initData.videoMode.width /
                        getCharWidth(initData.font, initData.fontSize);
    }

    if (initData.videoMode.height > sf::VideoMode::getDesktopMode().height)
    {
        initData.videoMode.height = sf::VideoMode::getDesktopMode().height;
        initData.rows = initData.videoMode.height /
                        getLineSpacing(initData.font, initData.fontSize);
    }
}

#ifdef _WIN32
#include <windows.h>
//Used to hide console window.
//Compiling as WindowedApp did not work out - program just did not work.
//FreeConsole has the same effect as WindowedApp.
//Thanks to the udit043 from stackoverflow, console can be hidden.
//http://stackoverflow.com/questions/622592/win32-programming-hiding-console-window
void Stealth()
{
    HWND Stealth;
    AllocConsole();
    Stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(Stealth,0);
}
#else
void Stealth()
{
    std::cout << "Stealth(): Hiding console is not implemented on this platform." << std::endl;
}
#endif
