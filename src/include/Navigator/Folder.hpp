#ifndef FOLDER_HPP
#define FOLDER_HPP

#include <Record.hpp>
#include <NamedItem.hpp>
#include <ItemWithPath.hpp>

#include <SFML/System/String.hpp>

#include <deque>

#include <memory>

class Folder: public NamedItem, public ItemWithPath
{
    public:
             Folder(const Folder *parent);

             //Folder(const Folder &copy);

        const Folder *
             getParent() const;

        const std::deque<std::shared_ptr<Folder>> & 
             getFolders() const;

        const std::deque<std::shared_ptr<Record>> & 
             getRecords() const;

        //for use in RecordSystem
        std::deque<std::shared_ptr<Folder>> & 
             getFolders();

        std::deque<std::shared_ptr<Record>> & 
             getRecords();

        bool scrollDown();
        bool scrollUp();

        void setScrollToLastFolder();
        void setScrollToLastRecord();

        int  getItemCount() const;
        int  getScroll() const;

        Folder * 
             addFolder();
        Record * 
             addRecord();

        Record * 
             addRecord(std::shared_ptr<Record> record);
        Folder * 
             addFolder(std::shared_ptr<Folder> folder);

        Folder* 
             addFolderIfNotExists(std::string path, 
                                  std::basic_string<sf::Uint32> name);

        const std::vector<const Folder *> 
             getPath() const;

        void resetScroll();

        void removeAllItems();

        void search(Folder *searchFolder, const std::basic_string<sf::Uint32> &query);

    private:
        static int
             count;

        std::deque<std::shared_ptr<Folder>> 
             mFolders; 

        std::deque<std::shared_ptr<Record>>
             mRecords; 

        const Folder *
             mParent;

        int  mScroll;
};

#endif
