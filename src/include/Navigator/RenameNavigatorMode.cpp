#include <RenameNavigatorMode.hpp>

#include <Debug.hpp>

RenameNavigatorMode::RenameNavigatorMode(RecordSystem &recordSystem, 
                                         sf::RenderTexture &renderTexture,
                                         NavigatorSignals &signals)
    : NavigatorMode(recordSystem, renderTexture, signals)
{

}

void RenameNavigatorMode::atSwitchingOn()
{
    auto itemInfo = getItemInfo(mView.currentItem);
    auto kind = itemInfo.first;

    LOG(switching on rename mode)

    if (kind != ItemKind::NestedRecord && kind != ItemKind::NestedFolder)
    {
        mSignals.switchToNormal.status = true;
        LOG(signalling to switch to normal mode from rename mode)
    }
}

void RenameNavigatorMode::processTasks()
{

}

void RenameNavigatorMode::updateFromRecordSystem()
{
    mCurrentFolder = mRecordSystem.getCurrentFolder();
    mPathToCurrentFolder = mRecordSystem.getPathToCurrentFolder();
}


void RenameNavigatorMode::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
        switch (event.key.code)
        {
            case sf::Keyboard::Key::Return:
                mSignals.switchToNormal.status = true;
                return;
        
            default:
                break;
        }


    auto itemInfo = getItemInfo(mView.currentItem);
    auto kind = itemInfo.first;
    auto index = itemInfo.second;

    NamedItem *renamedItem = nullptr;

    switch (kind)
    {
        case ItemKind::NestedFolder:
        renamedItem = mCurrentFolder -> getFolders()[index].get(); 
        break;

        case ItemKind::NestedRecord:
        renamedItem = mCurrentFolder -> getRecords()[index].get(); 
        break;

        default:
        return;
    }

    static bool backspacePressed = false;

    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Key::BackSpace)
        {
            if (!renamedItem -> getName().empty())
            {
                auto newName = renamedItem -> getName();
                newName.pop_back();
                renamedItem -> setName(newName);
            }

            mNeedUpdate = true;
            backspacePressed = true;
            return;
        }

    if (event.type == sf::Event::TextEntered)
    {
        if (backspacePressed)
            backspacePressed = false;
        else
        {
            renamedItem -> setName(renamedItem -> getName() + event.text.unicode);
            mNeedUpdate = true;
        }
    }
}
