#include <Nvim/Neovim.hpp>

#include <SFML/System/String.hpp>

#include <iostream>
#include <map>

#include <PythonBindings.hpp> //runBridge

nvim::Neovim::Neovim(const InitData &initData, Signals &signals)
    : mGUIOptions(initData.font, initData.fontSize, initData.rows, initData.cols)
    , mInitData(initData)
    , mSignals(signals)
    , mDataHolder(initData.rows, initData.cols)
    , mPictureProvider(mGUIOptions)
    , mTextRenderer(mDataHolder, mGUIOptions)
    , mBackgroundRenderer(mDataHolder, mGUIOptions)
    , mPictureRenderer(mDataHolder, mGUIOptions, mPictureProvider)
    , mNeedRedraw(true)
    , mNeedResize(false)
    , mBlinky(mDataHolder, mGUIOptions)
{
    bridgeData.neovim = this;
    bridgeData.dataHolder = &mDataHolder;
    bridgeData.newDimensionsRows = initData.rows;
    bridgeData.newDimensionsCols = initData.cols;
    bridgeData.commandLineArgument = initData.fileToOpen;

    mTextRenderer.setTextOffset(initData.xTextOffset, initData.yTextOffset);
    mBlinky.setTextOffset(initData.xTextOffset, initData.yTextOffset);

    auto bridgeRunner = [this] () { runBridge(bridgeData); };
    mPythonBridge = std::thread(bridgeRunner);
}

void nvim::Neovim::render(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    mBackgroundRenderer.render(target, states);
    mTextRenderer.render(target, states);
    mPictureRenderer.render(target, states);
    mBlinky.render(target, states);
}

void nvim::Neovim::update(sf::Time dt)
{
    bridgeData.updateMutex.lock();
    
    if (mNeedResize)
    {
        resize(mNewRowSize, mNewColSize);
        mNeedResize = false;
        bridgeData.resizeMutex.unlock();
    }

    if (mNeedRedraw)
    {
        mBackgroundRenderer.update();
        mTextRenderer.update();
        mPictureRenderer.update();
        mBlinky.update(dt, true); //true because layout has changed
        scheduleRedraw(false);
    }
    else
        mBlinky.update(dt);

    if (bridgeData.switchToNavigatorFlag)
    {
        bridgeData.switchToNavigatorFlag = false;
        mSignals.switchToNavigator.status = true;
    }

    bridgeData.updateMutex.unlock();
}

void nvim::Neovim::changeMode(nvim::Blinky::Mode mode)
{
    mBlinky.changeMode(mode); 
}

void nvim::Neovim::quit()
{
    exit(true);
}

void nvim::Neovim::setTitle(const char *title)
{
    mSignals.setTitle.status = true;
    mSignals.setTitle.title = title;
}

void nvim::Neovim::resize(int rows, int cols)
{
    //order matters
    mGUIOptions.resize(rows, cols);
    mDataHolder.resize(rows, cols);
    mBackgroundRenderer.resize();
    mTextRenderer.resize();
    mPictureRenderer.resize();
}

void nvim::Neovim::scheduleRedraw(bool f)
{
    mNeedRedraw = f;
}

void nvim::Neovim::scheduleResize(int rows, int cols)
{
    bridgeData.resizeMutex.lock();
    mNeedResize = true;
    mNewRowSize = rows;
    mNewColSize = cols;
}

void nvim::Neovim::handleEvent(const sf::Event &event)
{
    static std::map<sf::Keyboard::Key, std::string> KEY_MAP; 
    KEY_MAP[sf::Keyboard::Key::Up] = "<Up>";
    KEY_MAP[sf::Keyboard::Key::Down] = "<Down>";
    KEY_MAP[sf::Keyboard::Key::Left] = "<Left>";
    KEY_MAP[sf::Keyboard::Key::Right] = "<Right>";

    if (event.type == sf::Event::TextEntered)
    {
        bridgeData.nvimInputMutex.lock();

        if (sf::String(event.text.unicode) == "<")
            bridgeData.userInput += "<lt>";
        else
            bridgeData.userInput += event.text.unicode;

        bridgeData.nvimInputMutex.unlock();
    }
    else if (event.type == sf::Event::KeyPressed && KEY_MAP.find(event.key.code) != KEY_MAP.end())
    {
        bridgeData.nvimInputMutex.lock();

        bridgeData.userInput += KEY_MAP.find(event.key.code) -> second;

        bridgeData.nvimInputMutex.unlock();
    }
    
    checkForResize(event);

    //check for quit
    if (event.type == sf::Event::Closed)
        bridgeData.quitFlag = true;
}

void nvim::Neovim::openPath(std::string path)
{
    input("<ESC>:w<ENTER>:edit " + path);
    /*
    bridgeData.nvimInputMutex.lock();

    bridgeData.userInput += "<ESC>:w<ENTER>:edit " + path;

    bridgeData.nvimInputMutex.unlock();
    */
}

void nvim::Neovim::input(std::string query)
{
    bridgeData.nvimInputMutex.lock();

    bridgeData.userInput += query;

    bridgeData.nvimInputMutex.unlock();
}

void nvim::Neovim::checkForResize(const sf::Event &event)
{
    if (event.type == sf::Event::Resized)
    {
        bridgeData.newDimensionsRows = event.size.height / mGUIOptions.getLineSpacing();
        bridgeData.newDimensionsCols = event.size.width / mGUIOptions.getCharWidth();
    }
}
