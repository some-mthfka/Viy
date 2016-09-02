#ifndef ITEM_WITH_PATH_HPP
#define ITEM_WITH_PATH_HPP

#include <string>
#include <SFML/System/String.hpp>

class ItemWithPath 
{
    public:
        void setPath(sf::String name);

        const sf::String& 
             getPath() const;

    private:
        sf::String
             mPath;
};

#endif
