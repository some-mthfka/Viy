#ifndef NVIM_TAG_HPP
#define NVIM_TAG_HPP

#include <SFML/System/String.hpp>

#include <NamedItem.hpp>

#include <regex>

namespace nvim
{
    class Tag: public NamedItem
    {
        public:
                 Tag(const sf::String &row);

            int  getSize() const { return mSize; }
            int  getLength() const { return mLength; }
            int  getFragmentNumber() const { return mFragmentNumber; }
            int  getHorizontalPosition() const { return mHorizontalPosition; }

            bool isValid() { return mValid; }

        private:
            void readValues(const std::string &row);

        private:
            const static std::regex
                 tagRegex;

            int  mSize = -1,
                 mLength = -1,
                 mFragmentNumber = -1, 
                 mHorizontalPosition = -1;

            bool mValid = false;
    };
}

#endif
