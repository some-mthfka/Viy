#ifndef NVIM_DATA_HOLDER_HPP
#define NVIM_DATA_HOLDER_HPP

#include <vector>

#include <cwchar>

#include <cstdint>

#include <SFML/System/String.hpp>
#include <SFML/Graphics/Text.hpp>

#include <iostream>

/* Data holder keeps the screen grid which is modified by nvim and
 * scanned by screen renderers (back/foreground renderers, blinky)
 * through appropriate API.
 */

namespace nvim
{
    struct CellAttribute
    {
        enum class  Text  { NONE, BOLD, UNDERLINED, UNDERCURL, ITALIC };
        enum class  Color { NONE, FOREGROUND, BACKGROUND, SPECIAL, REVERSE };

        bool        reset = false; //is set when cell attrs need to be reset

        Text        textAttr  = Text::NONE;
        Color       colorAttr = Color::NONE;
        
        bool        state = false;
        sf::Color   color;
    };

    class DataHolder 
    {
        public:
                 DataHolder(int rows, int cols);

            //API for the Bridge to use
            void clear();
            void eolClear();
            void cursorGoto(int32_t row, int32_t col);
            void setScrollRegion(int32_t top, 
                                 int32_t bottom, 
                                 int32_t left, 
                                 int32_t right);
            void scroll(int32_t count);
            void put(std::basic_string<char32_t> text);
            void highlightSet(CellAttribute cellAttribute);

            void resize(int rows, int cols);

            void setPrimaryBackgroundColor(sf::Color color);
            void setPrimaryForegroundColor(sf::Color color);

            sf::Color
                 getPrimaryForegroundColor() const { return mPrimaryForegroundColor; }
            sf::Color
                 getPrimaryBackgroundColor() const { return mPrimaryBackgroundColor; }

            sf::Vector2i
                 getCursorPosition() const { return {mRow, mCol}; }

            const sf::String &
                 getTextAtRow(int row) const { return mText.at(row); }
            const std::vector<uint32_t> &
                 getTextAttributesAtRow(int row) const { return mTextAttributes.at(row); }
            const std::vector<sf::Color> &
                 getColorAttributeAtRow(int row) const { return mForeground.at(row); }
            const std::vector<sf::Color> &
                 getBackgroundColorsAtRow(int row) const { return mBackground.at(row); }

            bool
                 hasBackgroundChanged() { return mBackgroundChanged; };


            void printScreen()
            {
                for (int i = 0; i < mRows; i++)
                    std::cout << mText[i].toAnsiString() << std::endl;
            }
        private:
            void clearRegion(int32_t top, int32_t bottom, int32_t left, int32_t right);

        private:
            std::vector<sf::String> 
                 mText;

            std::vector<std::vector<uint32_t>>
                 mTextAttributes;

            std::vector<std::vector<sf::Color>>
                 mBackground;

            std::vector<std::vector<sf::Color>>
                 mForeground;

            uint32_t 
                 mPrintStyle;
            
            sf::Color
                 mPrimaryBackgroundColor, mPrimaryForegroundColor;

            sf::Color
                 mBackgroundPrintColor, mForegroundPrintColor;

            bool
                 mBackgroundChanged;

            int32_t 
                 mRows, mCols,                          //size
                 mRow = 0, mCol = 0,                    //current position 
                 mTop = 0, mBottom, mLeft = 0, mRight;  //scroll region

    };
} //end namespace nvim

#endif // NVIM_DATA_HOLDER_HPP
