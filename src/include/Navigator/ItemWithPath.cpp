#include <ItemWithPath.hpp>

void ItemWithPath::setPath(sf::String path)
{
    mPath = path;
}

const sf::String& ItemWithPath::getPath() const
{
    return mPath;
}
