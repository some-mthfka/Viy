#include <Nvim/DataHolder.hpp>

#include <cassert>
#include <iostream>

#include <algorithm> //swap_ranges

nvim::DataHolder::DataHolder(int rows, int cols)
    : mPrintStyle(sf::Text::Style::Regular)
    , mPrimaryBackgroundColor(sf::Color::White)
    , mPrimaryForegroundColor(sf::Color::Black)
    , mBackgroundPrintColor(mPrimaryBackgroundColor)
    , mForegroundPrintColor(mPrimaryForegroundColor)
    , mBackgroundChanged(false)
{
    resize(rows, cols);
} 


void nvim::DataHolder::clear()
{
    clearRegion(mTop, mBottom, mLeft, mRight);
}

void nvim::DataHolder::eolClear()
{
    clearRegion(mRow, mRow, mCol, mRight);
}

void nvim::DataHolder::cursorGoto(int32_t row, int32_t col)
{
    mRow = row;
    mCol = col;
}

void nvim::DataHolder::setScrollRegion(int32_t top, int32_t bottom, int32_t left, int32_t right)
{
    mTop = top;
    mBottom = bottom;
    mLeft = left;
    mRight = right;
}

void nvim::DataHolder::scroll(int32_t count)
{
    //Positive count indicates how many lines in the beginning
    //of the scroll region need to be replaced by the lines left in the bottom.
    //Negative count indicates how many lines in the end
    //of the scroll regino need to be replaced by the lines left on the top.
    
    auto swapRows = [this] (int first_row, int second_row)
    {
        //this should be pretty efficient thanks to the move semantics
        std::swap_ranges(mText[first_row].begin() + mLeft, 
                         mText[first_row].begin() + mRight + 1, 
                         mText[second_row].begin() + mLeft);

        std::swap_ranges(mTextAttributes[first_row].begin() + mLeft, 
                         mTextAttributes[first_row].begin() + mRight + 1,
                         mTextAttributes[second_row].begin() + mLeft);

        std::swap_ranges(mBackground[first_row].begin() + mLeft, 
                         mBackground[first_row].begin() + mRight + 1, 
                         mBackground[second_row].begin() + mLeft);

        std::swap_ranges(mForeground[first_row].begin() + mLeft, 
                         mForeground[first_row].begin() + mRight + 1,
                         mForeground[second_row].begin() + mLeft);
    };
    
    if (count > 0)
    {
        for (int row = mTop + count; row <= mBottom; row++)
            swapRows(row, row - count);

        clearRegion(mBottom - count + 1, mBottom, mLeft, mRight);  
    }
    else
    {
        count = abs(count);
        for (int row = mBottom - count; row >= mTop; row--)
            swapRows(row, row + count);

        clearRegion(mTop, mTop + count - 1, mLeft, mRight);  
    }
}

void nvim::DataHolder::put(std::basic_string<char32_t> text)
{
    //Possible OPTIMIZATION of replacing "at" with []
    for (char32_t &c: text) 
    {
        mText.at(mRow)[mCol] = c;
        mTextAttributes.at(mRow).at(mCol) = mPrintStyle;
        mForeground.at(mRow).at(mCol) = mForegroundPrintColor;
        mBackground.at(mRow).at(mCol) = mBackgroundPrintColor;
        cursorGoto(mRow, mCol + 1);
    }
}

void nvim::DataHolder::highlightSet(CellAttribute attr)
{
    typedef nvim::CellAttribute::Text TextAttr;
    typedef nvim::CellAttribute::Color ColorAttr;

    if (attr.reset)
    {
        mPrintStyle = sf::Text::Style::Regular;
        mForegroundPrintColor = mPrimaryForegroundColor;
        mBackgroundPrintColor = mPrimaryBackgroundColor;
        return;
    }

    assert(attr.textAttr != TextAttr::NONE || attr.colorAttr != ColorAttr::NONE); 

    std::map<TextAttr, sf::Text::Style> textStyleMapping;
    textStyleMapping[TextAttr::BOLD] = sf::Text::Style::Bold;
    textStyleMapping[TextAttr::UNDERLINED] = sf::Text::Style::Underlined;
    textStyleMapping[TextAttr::UNDERCURL] = sf::Text::Style::Underlined; //Let undercurl = underlined
    textStyleMapping[TextAttr::ITALIC] = sf::Text::Style::Italic;
    //textStyleMapping[TextAttr::REVERSE] = sf::Text::Style::Bold;

    if (attr.textAttr != TextAttr::NONE)
    {
        if (attr.state)
            mPrintStyle |= textStyleMapping[attr.textAttr];
        else
            mPrintStyle &= ~textStyleMapping[attr.textAttr];
    }

    if (attr.colorAttr != ColorAttr::NONE)
    {
        if (attr.colorAttr == ColorAttr::FOREGROUND)
            mForegroundPrintColor = attr.color;

        if (attr.colorAttr == ColorAttr::BACKGROUND)
            mBackgroundPrintColor = attr.color;

        if (attr.colorAttr == ColorAttr::REVERSE)
        {
            mForegroundPrintColor = sf::Color(255 - mForegroundPrintColor.r,
                                              255 - mForegroundPrintColor.g,
                                              255 - mForegroundPrintColor.b,
                                              255); 

            mBackgroundPrintColor = sf::Color(255 - mBackgroundPrintColor.r,
                                              255 - mBackgroundPrintColor.g,
                                              255 - mBackgroundPrintColor.b,
                                              255); 
        }
            
        //color stuff
    }
}

void nvim::DataHolder::resize(int rows, int cols)
{
    //Resizing screen clears everything

    //Need to allocate a vector of empty strings - our grid is then ready
    //for new characters from neovim.

    assert(rows > 0);
    assert(cols > 0);
    
    mRows = rows;
    mCols = cols;

    mBottom = rows - 1;
    mRight = cols - 1;

    //The following four lines are not really needed
    mText.resize(0);
    mTextAttributes.resize(0);
    mBackground.resize(0);
    mForeground.resize(0);

    sf::String emptyRow;
    for (int i = 0; i < cols; i++)
        emptyRow += ' ';

    mText.resize(rows);
    for (int i = 0; i < rows; i++)
        mText[i] = emptyRow;

    mTextAttributes.resize(rows);
    for (int i = 0; i < rows; i++)
        mTextAttributes[i].resize(cols, sf::Text::Style::Regular);

    mBackground.resize(rows);
    for (int i = 0; i < rows; i++)
        mBackground[i].resize(cols, mPrimaryBackgroundColor);

    mForeground.resize(rows);
    for (int i = 0; i < rows; i++)
        mForeground[i].resize(cols, mPrimaryForegroundColor);

    //I dunno why, but Nnvim does not send a cursorGoto(0, 0) itself
    //(but apparently wants it this way)
    cursorGoto(0, 0);
}

void nvim::DataHolder::setPrimaryBackgroundColor(sf::Color color)
{
    mPrimaryBackgroundColor = color;
}

void nvim::DataHolder::setPrimaryForegroundColor(sf::Color color)
{
    mPrimaryForegroundColor = color;
}

//private
void nvim::DataHolder::clearRegion(int32_t top, int32_t bottom, int32_t left, int32_t right)
{
    //Clear the cells in a rectangular region given in the arguments
    for (int32_t i = top; i <= bottom; i++)
        for (int32_t j = left; j <= right; j++)
        {
            mText.at(i)[j] = ' '; //, Cell::Attributes());
            mTextAttributes.at(i).at(j) = 0;
            mForeground.at(i).at(j) = mPrimaryForegroundColor;
            mBackground.at(i).at(j) = mPrimaryBackgroundColor;
        }
}
