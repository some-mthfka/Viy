#include <Nvim/Blinky.hpp>

nvim::Blinky::Blinky(nvim::DataHolder &dataHolder, nvim::GUIOptions &GUIOptions)
    : mDataHolder(dataHolder)
    , mGUIOptions(GUIOptions)
    , mBlinkPeriod(sf::milliseconds(500))
    , mIsON(true)
    , mModeChanged(false)
{
    setTextOffset(0, 0);

    mCursor.setFillColor(sf::Color::Black); 

    mDrawableChar.setString(" ");
    mDrawableChar.setFont(mGUIOptions.getFont());
    mDrawableChar.setCharacterSize(mGUIOptions.getFontSize());

    changeMode(Mode::NORMAL);
}

void nvim::Blinky::update(sf::Time dt, bool layoutChanged)
{
    if (layoutChanged || mTimer.getElapsedTime() > mBlinkPeriod || mModeChanged)
    {
        mIsON = !mIsON; //switch state

        if (layoutChanged || mModeChanged)
        {
            mIsON = true;

            mModeChanged = false;
        }

        if (mIsON)
        {
            auto pos = mDataHolder.getCursorPosition();
            auto row = pos.x;
            auto col = pos.y;

            mCursor.setPosition(col * mGUIOptions.getCharWidth(), 
                                row * mGUIOptions.getLineSpacing());

            mCursor.setFillColor(mDataHolder.getPrimaryForegroundColor());

            mDrawableChar.setString(mDataHolder.getTextAtRow(row)[col]);
            mDrawableChar.setStyle(mDataHolder.getTextAttributesAtRow(row)[col]);
            mDrawableChar.setColor(mDataHolder.getPrimaryBackgroundColor());
            mDrawableChar.setPosition(mCursor.getPosition() + sf::Vector2f(mOffsetX, mOffsetY));
        }

        mTimer.restart();
    }
}

void nvim::Blinky::render(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    if (mIsON)
    {
        target.draw(mCursor, states);

        if (mMode == Mode::NORMAL)
            target.draw(mDrawableChar, states);
    }
}

void nvim::Blinky::changeMode(Mode mode)
{
    mMode = mode;
    mModeChanged = true;
    if (mode == Mode::NORMAL)
        mCursor.setSize({static_cast<float>(mGUIOptions.getCharWidth()), 
                         static_cast<float>(mGUIOptions.getLineSpacing())});
    else
        mCursor.setSize({static_cast<float>(ceil(mGUIOptions.getCharWidth() / 5.0)), 
                         static_cast<float>(mGUIOptions.getLineSpacing())});
}

void nvim::Blinky::setTextOffset(int x, int y)
{
    mOffsetX = x;
    mOffsetY = y;
}
