#include <NormalNavigatorMode.hpp>

NormalNavigatorMode::NormalNavigatorMode(RecordSystem &recordSystem, 
                                         sf::RenderTexture &renderTexture,
                                         NavigatorSignals &signals)
    : NavigatorMode(recordSystem, renderTexture, signals)
{

}

void NormalNavigatorMode::processTasks()
{
    if (mTasks.addFolder)
    {
        mTasks.addFolder = false;
        mCurrentFolder -> addFolder();
        mCurrentFolder -> setScrollToLastFolder();
    }

    if (mTasks.addRecord)
    {
        mTasks.addRecord = false;
        mCurrentFolder -> addRecord();
        mCurrentFolder -> setScrollToLastRecord();
    }
}

void NormalNavigatorMode::updateFromRecordSystem()
{
    mCurrentFolder = mRecordSystem.getCurrentFolder();
    mPathToCurrentFolder = mRecordSystem.getPathToCurrentFolder();
}


void NormalNavigatorMode::handleEvent(const sf::Event &event)
{
    NavigatorMode::handleEvent(event);

    if (event.type == sf::Event::KeyPressed)
        switch (event.key.code)
        {
            /*
            case sf::Keyboard::Key::T: 
                mTasks.addFolder = true;
                mNeedUpdate = true;
                break;

            case sf::Keyboard::Key::N: 
                mTasks.addRecord = true;
                mNeedUpdate = true;
                break;
                */

            default:
                break;
        }
}
