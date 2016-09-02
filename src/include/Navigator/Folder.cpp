#include <Folder.hpp>

#include <Utility.hpp>

#include <cassert>

#include <Debug.hpp>

int Folder::count = 0;

Folder::Folder(const Folder *parent)
    : mParent(parent)
    , mScroll(0)
{
    setName(sf::String("Folder ").toUtf32() + sf::String(toString(Folder::count)).toUtf32());

    Folder::count++;
}

const Folder * Folder::getParent() const
{
    return mParent;
}

const std::deque<std::shared_ptr<Folder>> & Folder::getFolders() const
{
    return mFolders;
}

std::deque<std::shared_ptr<Folder>> & Folder::getFolders()
{
    return mFolders;
}

const std::deque<std::shared_ptr<Record>> & Folder::getRecords() const
{
    return mRecords;
}

std::deque<std::shared_ptr<Record>> & Folder::getRecords()
{
    return mRecords;
}

int Folder::getScroll() const
{
    return mScroll;
}

bool Folder::scrollDown()
{
    if (mScroll < getItemCount())
    {
        mScroll++;
        return true;
    }

    return false;
}

bool Folder::scrollUp()
{
    if (mScroll > -1 * static_cast<int>(getPath().size()))
    {
        mScroll--;
        return true;
    }

    return false;
}

void Folder::setScrollToLastFolder()
{
    mScroll = mFolders.size() - 1;
    assert(mScroll >= 0);
}

void Folder::setScrollToLastRecord()
{
    mScroll = mFolders.size() + mRecords.size() - 1;
    assert(mScroll >= 0);
}

int Folder::getItemCount() const
{
    return mFolders.size() + mRecords.size();
}

Record* Folder::addRecord()
{
    mRecords.push_back(std::make_shared<Record>());
    return mRecords.back().get();
}

Folder* Folder::addFolder()
{
    mFolders.push_back(std::make_shared<Folder>(this));
    return &*mFolders.back().get();
}

Record* Folder::addRecord(std::shared_ptr<Record> record)
{
    mRecords.push_back(record);
    return &*mRecords.back().get();
}

Folder* Folder::addFolder(std::shared_ptr<Folder> folder)
{
    mFolders.push_back(folder);
    return &*mFolders.back().get();
}

Folder* Folder::addFolderIfNotExists(std::string path, 
                                     std::basic_string<sf::Uint32> name)
{
    for (auto &folder: mFolders)
        if (folder -> getName() == name)
            return folder.get();

    Folder *addedFolder = addFolder();
    addedFolder -> setPath(path);
    addedFolder -> setName(name);

    return addedFolder;
}


const std::vector<const Folder *> Folder::getPath() const
{
    std::vector<const Folder *> pathToCurrentFolder;

    const Folder *folder = this;

    while ((folder = folder -> getParent()))
        pathToCurrentFolder.push_back(folder);

    std::reverse(pathToCurrentFolder.begin(), pathToCurrentFolder.end());

    return pathToCurrentFolder;
}

void Folder::resetScroll()
{
    mScroll = 0;
}

//for the search folder
void Folder::removeAllItems()
{
    mFolders.clear();
    mRecords.clear();
}

void Folder::search(Folder *searchFolder, const std::basic_string<sf::Uint32> &query)
{
    if (query.empty())
        return;

    for (auto &folder: mFolders)
        if (folder -> getName().find(query) != std::basic_string<sf::Uint32>::npos)
            searchFolder -> addFolder(folder);

    for (auto &record: mRecords)
        if (record -> getName().find(query) != std::basic_string<sf::Uint32>::npos)
            searchFolder -> addRecord(record);

    for (auto &folder: mFolders)
        folder -> search(searchFolder, query);
}
