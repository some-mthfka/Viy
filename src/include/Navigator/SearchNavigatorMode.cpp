#include <SearchNavigatorMode.hpp>

#include <Debug.hpp>

SearchNavigatorMode::SearchNavigatorMode(RecordSystem &recordSystem, 
                                         sf::RenderTexture &renderTexture,
                                         NavigatorSignals &signals)
    : NavigatorMode(recordSystem, renderTexture, signals)
{

}

void SearchNavigatorMode::atSwitchingOn()
{
    mSearchQuery.clear();
}

void SearchNavigatorMode::atSwitchingOff()
{
    mRecordSystem.restoreFolderBeforeSearch();
}

void SearchNavigatorMode::processTasks()
{

}

void SearchNavigatorMode::updateFromRecordSystem()
{
    mCurrentFolder = mRecordSystem.search(mSearchQuery);
    mPathToCurrentFolder.clear();
}

void SearchNavigatorMode::handleEvent(const sf::Event &event)
{
    NavigatorMode::handleEvent(event);

    static bool backspacePressed = false;

    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Key::BackSpace)
        {
            if (!mSearchQuery.empty())
                mSearchQuery.pop_back();
            mNeedUpdate = true;
            backspacePressed = true;
            return;
        }


    if (event.type == sf::Event::TextEntered)
    {
        if (backspacePressed)
        {
            backspacePressed = false;
            return;
        }

        switch (event.text.unicode)
        {
            case '/': case 13: //13 is return, better use whitespace
                break;

            default:
                mSearchQuery += event.text.unicode;
                mNeedUpdate = true;
                LOG(ADDING A CHAR TO QUERY)
                break;
        }
    }
}
