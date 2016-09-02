#include <RecordSystem.hpp>

#include <algorithm>
#include <cassert>

RecordSystem::RecordSystem()
    : mRootFolder(/*parent =*/ nullptr)
    , mSearchFolder(nullptr)
    , mActiveFolder(&mRootFolder)
    , mFolderBeforeSearch(mActiveFolder)
{
    //for (int i = 0; i < 3; i++)
        //mRootFolder.addFolder();

    //for (int i = 0; i < 5; i++)
        //mRootFolder.addRecord();
}

const std::vector<const Folder *> RecordSystem::getPathToCurrentFolder() const
{
    //should probably remove this method
    return mActiveFolder -> getPath();
}

Folder * RecordSystem::getCurrentFolder() const
{
    return mActiveFolder;
}

void RecordSystem::enterParentFolder(unsigned int index)
{
    mActiveFolder = const_cast<Folder *>(getPathToCurrentFolder()[index]);
}

void RecordSystem::enterNestedFolder(unsigned int index)
{
    mActiveFolder = mActiveFolder -> getFolders() [index].get();
}

Folder * RecordSystem::search(std::basic_string<sf::Uint32> query)
{
    if (mActiveFolder != &mSearchFolder)
        mFolderBeforeSearch = mActiveFolder;

    mSearchFolder.removeAllItems();
    mSearchFolder.resetScroll();
    mSearchFolder.setName(query);

    mActiveFolder = &mSearchFolder;

    mRootFolder.search(&mSearchFolder, query);
    
    return &mSearchFolder;
}

void RecordSystem::restoreFolderBeforeSearch()
{
    mActiveFolder = mFolderBeforeSearch;
}
