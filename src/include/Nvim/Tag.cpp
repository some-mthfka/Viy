#include <Nvim/Tag.hpp>

#include <cassert>
#include <sstream>

#include <Debug.hpp>

//The regex could for example match <|cool pic 99.jpg|11|3|>.
const std::regex nvim::Tag::tagRegex("^<\\|(\\w| |/)*.\\w*\\|\\d{1,2}\\|\\d{1,2}\\|>.*");

nvim::Tag::Tag(const sf::String &row) 
{
    std::string ansiRow(row.toAnsiString());

    std::size_t tagStartPosition;

    do
    {
        //Expression may not necessarily be in the beginning of line.
        tagStartPosition = ansiRow.find('<');

        if (tagStartPosition == std::string::npos)
            return;

        ansiRow.erase(0, tagStartPosition);

        //assert(std::regex_match("<|cool pic 99.jpg|11|3|>", tagRegex));

        if (std::regex_match(ansiRow, tagRegex))
        {
            mHorizontalPosition = row.getSize() - ansiRow.size();
            readValues(ansiRow);

            mLength = ansiRow.find('>') + 1;

            return;
        }
        else
            ansiRow.erase(0, 1);
    }
    while (true);
}

void nvim::Tag::readValues(const std::string &row)
{
    std::stringstream tag(row), bufss;

    const int size = 512;
    char buffer[size];
    const char delimiter = '|';

    char buf;

    tag >> buf >> buf; //remove <|

    tag.getline(buffer, size, delimiter);
    setName(sf::String(buffer).toUtf32());

    tag.getline(buffer, size, delimiter);
    bufss << buffer << ' ';
    bufss >> mSize;

    if (mSize <= 0)
        return;

    tag.getline(buffer, size, delimiter);
    bufss << buffer << ' ';
    bufss >> mFragmentNumber;

    if (mFragmentNumber < 0 || mFragmentNumber >= mSize)
        return;

    mValid = true;
}
