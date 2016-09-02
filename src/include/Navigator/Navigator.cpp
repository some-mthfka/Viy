#include <Navigator.hpp>

#include <cassert>

#include <SFML/Graphics/Text.hpp>

#include <RecordSystemFiller.hpp>
#include <Debug.hpp>

Navigator::Navigator(NavigatorSignals &signals)
    : mSignals(signals)
    , mMode(NavigatorMode::Mode::Normal)
    , mNormalMode(mRecordSystem, mRenderTexture, mSignals)
    , mSearchMode(mRecordSystem, mRenderTexture, mSignals)
    , mRenameMode(mRecordSystem, mRenderTexture, mSignals)
    , mCurrentMode(&mNormalMode)
{
    fillRecordSystem(mRecordSystem);
}

void Navigator::render(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    mCurrentMode -> render(target, states);
}

void Navigator::update(sf::Time dt)
{
    mCurrentMode -> update(dt);
}

void Navigator::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::Resized)
    {
        auto width = event.size.width,
             height = event.size.height;

        mCurrentMode -> resize(width, height);
    }

    if (event.type == sf::Event::KeyPressed)
        switch (event.key.code)
        {
            case sf::Keyboard::Key::Slash: 
                if (mCurrentMode != &mRenameMode)
                    switchToMode(mSearchMode);
                break;

            case sf::Keyboard::Key::Escape:
                switchToMode(mNormalMode);
                break;

            case sf::Keyboard::Key::F2:
                if (mCurrentMode != &mSearchMode)
                    switchToMode(mRenameMode);
                break;

            case sf::Keyboard::Key::Q:
                if (mCurrentMode != &mSearchMode && mCurrentMode != &mRenameMode)
                    mSignals.quit.status = true;
                break;

            default: 
                break;
        }
    
    mCurrentMode -> handleEvent(event);

    if (mSignals.switchToNormal.status)
    {
        mSignals.switchToNormal.status = false;
        mCurrentMode = &mNormalMode;
        mCurrentMode -> refresh();
    }
}

void Navigator::switchToMode(NavigatorMode &mode)
{
    auto currentSize = mCurrentMode -> getSize();
    mCurrentMode -> atSwitchingOff();
    mCurrentMode = &mode;
    mCurrentMode -> atSwitchingOn();
    mCurrentMode -> resize(currentSize.x, currentSize.y);
    mCurrentMode -> refresh();
}
