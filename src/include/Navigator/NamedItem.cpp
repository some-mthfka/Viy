#include <NamedItem.hpp>

void NamedItem::setName(std::basic_string<sf::Uint32> name)
{
    mName = name;
}

const std::basic_string<sf::Uint32>& NamedItem::getName() const
{
    return mName;
}
