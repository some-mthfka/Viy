#include <MainState.hpp>

#include <iostream>

#include <Utility.hpp>

#include <Debug.hpp>

MainState::MainState(StateStack& stack, State::Context context)
    : State(stack, context)
    , mNavigator(mNavigatorSignals)
{
    mInitData.font = mFontLoader.font;
    mInitData.fontSize = 24;
    mInitData.rows = 24;
    mInitData.cols = 80;
    mInitData.xTextOffset = 0;
    mInitData.yTextOffset = -2;
    
    constructVideoMode(mInitData); //called after font is loaded

    mNeovim.reset(new nvim::Neovim(mInitData, mNeovimSignals));

    resize(context.window -> getSize().x, context.window -> getSize().y);
}

void MainState::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
    //Probably need to add states with navigator/neovim states
    if (mFocusedOnNvim)
        mNeovim -> render(target, states);
    else
        mNavigator.render(target, states);
}

bool MainState::update(sf::Time dt)
{
    mNavigator.update(dt);
    mNeovim -> update(dt);

    if (mMonitorScreenshotHotkey && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F5))
        getContext().signals -> switchToSnapper.status = true;

	return true;
}

void MainState::resize(int width, int height)
{
    sf::Event resized;
    resized.type = sf::Event::Resized;

    //adjusting navigator
    resized.size.width = width;// * layout.navigatorWidthShare;
    resized.size.height = height;// * layout.navigatorHeightShare;
    mNavigator.handleEvent(resized);

    //adjusting neovim
    resized.size.width = width;// * layout.neovimWidthShare;
    resized.size.height = height;// * layout.neovimHeightShare;
    mNeovim -> handleEvent(resized);
}

bool MainState::handleEvent(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::LostFocus:
            mMonitorScreenshotHotkey = true;
            break;

        case sf::Event::GainedFocus:
            mMonitorScreenshotHotkey = false;
            break;

        case sf::Event::Resized:
            resize(event.size.width, event.size.height);
            return true;

        case sf::Event::Closed:
            mNeovim -> handleEvent(event);
            return true;

        case sf::Event::KeyPressed:
            switch(event.key.code)
            {
                case sf::Keyboard::F5:
                    getContext().signals -> switchToSnapper.status = true;
                    break;

                case sf::Keyboard::F11:
                    getContext().signals -> switchWindowType.status = true;
                    break;

                case sf::Keyboard::I:
                    mFocusedOnNvim = true;
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }

    if (mNavigatorSignals.enterRecord.status)
    {
        mNavigatorSignals.enterRecord.status = false;
        mFocusedOnNvim = true;
        mNeovim -> openPath(mNavigatorSignals.enterRecord.record -> getPath());
    }

    if (mNavigatorSignals.quit.status)
    {
        mNavigatorSignals.quit.status = false;
        sf::Event quit;
        quit.type = sf::Event::Closed;
        mNeovim -> handleEvent(quit);
        //need to switch to Neovim to receive quit response from the neovim instance
        mFocusedOnNvim = true;
        return true;
    }

    if (mNeovimSignals.switchToNavigator.status)
    {
        mNeovimSignals.switchToNavigator.status = false;
        mFocusedOnNvim = false;
        mNeovim -> input("<ESC>:w<CR>");
    }

    if (mFocusedOnNvim)
        mNeovim -> handleEvent(event);
    else
        mNavigator.handleEvent(event);

	return true;
}
