#ifndef RECORD_SYSTEM_HPP
#define RECORD_SYSTEM_HPP

#include <Folder.hpp>

#include <string>

class RecordSystem 
{
    public:
             RecordSystem();

             //make const
        Folder * 
             getCurrentFolder() const;

        const std::vector<const Folder *> 
             getPathToCurrentFolder() const;

        void enterParentFolder(unsigned int index);
        void enterNestedFolder(unsigned int index);

        Folder *
             search(std::basic_string<sf::Uint32> query);

        void restoreFolderBeforeSearch();

    private:
        Folder  
             mRootFolder,
             mSearchFolder;

        Folder 
            *mActiveFolder,
            *mFolderBeforeSearch;

        //bool searchMode = false;
};

#endif
