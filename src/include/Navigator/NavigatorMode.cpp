#include <NavigatorMode.hpp>

#include <cassert>

#include <Debug.hpp>

NavigatorMode::NavigatorMode(RecordSystem &recordSystem, 
                             sf::RenderTexture &renderTexture, 
                             NavigatorSignals &signals)
    : mRecordSystem(recordSystem)
    , mSignals(signals)
    , mRenderer(*this, renderTexture)
{

}

void NavigatorMode::render(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    mRenderer.render(target, states);
}

void NavigatorMode::adjustView()
{
    mView.currentItem = mPathToCurrentFolder.size() + mCurrentFolder -> getScroll();

    assert(mView.currentItem >= 0);

    mView.totalItems = mPathToCurrentFolder.size() 
                       + 1 
                       + mCurrentFolder -> getItemCount();

    mView.topItem = mView.currentItem - mView.maxItems / 2;

    if (mView.topItem + mView.maxItems > mView.totalItems)
        mView.topItem = mView.totalItems - mView.maxItems;

    if (mView.topItem < 0)
        mView.topItem = 0;

    if (mView.currentItem == static_cast<int>(mPathToCurrentFolder.size()))
        scrollDown();
}

void NavigatorMode::update(sf::Time dt)
{
    if (mNeedUpdate)
    {
        mNeedUpdate = false;
        
        processTasks();

        updateFromRecordSystem();

        adjustView();

        mRenderer.updateRenderTexture();
    }
}

void NavigatorMode::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
        switch (event.key.code)
        {
            case sf::Keyboard::Key::Up: 
                scrollUp(); 
                return;

            case sf::Keyboard::Key::Down: 
                scrollDown(); 
                return;

            case sf::Keyboard::Key::Return: 
                enter(); 
                return;

            default: 
                break;
        }
}

void NavigatorMode::atSwitchingOn()
{

}

void NavigatorMode::atSwitchingOff()
{

}

std::pair<NavigatorMode::ItemKind, unsigned int> NavigatorMode::getItemInfo(unsigned int index) const
{
    if (index < mPathToCurrentFolder.size())
        return { ItemKind::ParentFolder, index };

    index -= mPathToCurrentFolder.size();

    if (index == 0)
        return { ItemKind::CurrentFolder, index };

    index--;

    if (index < mCurrentFolder -> getFolders().size())
        return { ItemKind::NestedFolder, index };

    index -= mCurrentFolder -> getFolders().size();

    if (index < mCurrentFolder -> getRecords().size())
        return { ItemKind::NestedRecord, index };

    TRACE(mView.currentItem)
    TRACE(mView.totalItems)
    TRACE(index)
    assert(false);
}

void NavigatorMode::scrollDown()
{
    //if (mView.currentItem < mView.totalItems - 1)
    if (mCurrentFolder -> scrollDown())
    {
        mView.currentItem++;

        if (mView.currentItem >= mView.topItem + mView.maxItems &&
            mView.topItem + mView.maxItems < mView.totalItems)
                mView.topItem++;

        //mCurrentFolder -> scrollDown();

        mRenderer.updateRenderTexture();
    }

    //current folder is not selectable
    if (static_cast<unsigned int>(mView.currentItem) == mPathToCurrentFolder.size())
    {
        if (mCurrentFolder -> getItemCount() > 0)
            scrollDown();
        else 
        if (mPathToCurrentFolder.size() > 0)
            scrollUp();
    }
}

void NavigatorMode::scrollUp()
{
    //if (mView.currentItem > 0)
    if (mCurrentFolder -> scrollUp())
    {
        mView.currentItem--;

        if (mView.currentItem < mView.topItem && mView.topItem > 0)
            mView.topItem--;

        //mCurrentFolder -> scrollUp();

        mRenderer.updateRenderTexture();
    }

    if (static_cast<unsigned int>(mView.currentItem) == mPathToCurrentFolder.size())
    {
        if (mPathToCurrentFolder.size() > 0)
            scrollUp();
        else
            scrollDown();
    }
}

void NavigatorMode::enter()
{
    LOG(enter)
    TRACE(mView.currentItem)

    auto itemInfo = getItemInfo(mView.currentItem);
    auto kind = itemInfo.first;
    auto index = itemInfo.second;

    if (kind == ItemKind::ParentFolder)
    {
        mCurrentFolder -> resetScroll();
        mRecordSystem.enterParentFolder(index);
        mNeedUpdate = true;
    }

    if (kind == ItemKind::NestedFolder)
    {
        mRecordSystem.enterNestedFolder(index);

        //LOG(nav mode sending enter status switch to normal)
        mSignals.switchToNormal.status = true;

        mNeedUpdate = true;
    }

    if (kind == ItemKind::NestedRecord)
    {
        mSignals.enterRecord.status = true;
        mSignals.enterRecord.record = mCurrentFolder -> getRecords()[index].get();
    }
}

void NavigatorMode::resize(int width, int height)
{
    mRenderer.resize(width, height);

    mView.maxItems = mRenderer.howManyElementsFit();

    mRenderer.updateRenderTexture();
}
