#ifndef NAMED_ITEM_HPP
#define NAMED_ITEM_HPP

#include <string>
#include <SFML/System/String.hpp>

class NamedItem
{
    public:
        void setName(std::basic_string<sf::Uint32> name);

        const std::basic_string<sf::Uint32>& 
             getName() const;

    private:
        std::basic_string<sf::Uint32>
             mName;

};

#endif
