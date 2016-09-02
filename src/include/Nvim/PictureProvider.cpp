#include <Nvim/PictureProvider.hpp>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>

#include <fstream>
#include <exception>
#include <iostream>
#include <sstream>
#include <exception>
#include <cassert>

#include <Debug.hpp>

PictureProvider::PictureProvider(const nvim::GUIOptions &GUIOptions) 
{
    //Loading autoload info
    
    mAutoloadingInProgress = true;
    
    std::string filename = "library/images/Autoload.txt";
    std::ifstream in(filename);

    if (!in.is_open())
        throw std::runtime_error("Failed to load " + filename);
    else
    {
        const int size = 512;
        char buffer[size];

        std::stringstream str;

        in.getline(buffer, size);

        //remove path to library, use relative stuff only
        //std::string pathToLibrary(buffer);

        std::vector<std::string>
             picturesToLoad;

        while (in.getline(buffer, size))
            if (buffer[0] != '\0' && (buffer[0] != '-' || buffer[1] != '-')) //not comment or empty line
                picturesToLoad.push_back(buffer);

        for (const auto &file: picturesToLoad)
        {
            sf::Texture texture;
            if (texture.loadFromFile(/*pathToLibrary +*/ "library/images/" + file));
                mLoadedPictures[file] = texture;
        }
    }

    mAutoloadingInProgress = false;
}

const std::pair<PictureProvider::PictureLoadStatus, const sf::Texture *> 
PictureProvider::getPicture(sf::String name)
{
    if (mAutoloadingInProgress)
        return std::make_pair(PictureLoadStatus::LOADING, nullptr);

    auto found = mLoadedPictures.find(name);

    if (found != mLoadedPictures.end())
        return std::make_pair(PictureLoadStatus::READY, &(found -> second));
    else
    {
        sf::Texture texture;

        if (texture.loadFromFile(sf::String("library/images/") + name))
        {
            mLoadedPictures[name.toAnsiString()] = texture;
            return std::make_pair(PictureLoadStatus::READY, 
                                  &mLoadedPictures[name.toAnsiString()]);
        }
        else 
            return std::make_pair(PictureLoadStatus::FAILED, nullptr);
    }
}
