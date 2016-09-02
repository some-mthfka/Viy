#ifndef NVIM_GUI_OPTIONS_HPP
#define NVIM_GUI_OPTIONS_HPP

#include <Utility.hpp>

#include <iostream>
#include <cassert>

/* GUIOptions struct contains some general info about a set
 * of graphical properties needed by renderers.
 * Maintained by nvim::Neovim.
 */

namespace nvim
{
    struct GUIOptions
    {
        public:
            GUIOptions(const sf::Font &font, int fontSize, int rows, int cols) 
                : mFontSize(fontSize)
            { 
                setFont(font);
                resize(rows, cols);
            }

            void setFont(const sf::Font &font) 
            {
                mFont = &font;
                mCharWidth = ::getCharWidth(font, mFontSize);
                mLineSpacing = ::getLineSpacing(font, mFontSize);
            }

            void resize(int rows, int cols) { assert(cols > 0); assert(rows > 0); 
                                               mCols = cols; mRows = rows; }

            const sf::Font& 
                 getFont() const { return *mFont; }
            int  getFontSize() const { return mFontSize; }
            int  getLineSpacing() const { return mLineSpacing; }
            int  getCharWidth() const { return mCharWidth; }
            int  rows() const { return mRows; }
            int  cols() const { return mCols; }

        private:
            const sf::Font *mFont;
            int mFontSize;
            int mLineSpacing;
            int mCharWidth;
            int mRows, mCols;
    };
}

#endif
