#include <RecordSystemFiller.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <cassert>

#include <Debug.hpp>

void addItemToRecordSystem(RecordSystem &recordSystem, const std::string &pathToLibrary, std::string path)
{
    Folder *folderToAddTo = recordSystem.getCurrentFolder();

    const char delimiter = '/';

    //Remove assert, throw instead
    //assert(!path.empty());
    if (path.empty())
        throw std::runtime_error("Error while reading library structure.");

    std::stringstream ss(path);

    const int size = 512;
    char buffer[size];

    std::vector<std::string> items;
    while (ss.getline(buffer, size, delimiter))
        items.push_back(buffer);

    std::string recordToAdd;
    if (*path.rbegin() != delimiter)
    {
        recordToAdd = items.back();
        items.pop_back();
    }

    for (auto itr = items.begin(); itr != items.end(); itr++)
    {
        //LOG(adding a folder)
        //TRACE(*itr)
        folderToAddTo = folderToAddTo -> addFolderIfNotExists(path, sf::String(*itr).toUtf32());
    }

    if (!recordToAdd.empty())
    {
        //LOG(adding a record)
        //TRACE(recordToAdd)
        auto addedRecord = folderToAddTo -> addRecord();
        addedRecord -> setName(sf::String(recordToAdd).toUtf32());
        addedRecord -> setPath(pathToLibrary + path);
    }
}

void fillRecordSystem(RecordSystem &recordSystem)
{
    std::string filename = "library/LibraryStructure.txt";
    std::ifstream in(filename);

    if (!in.is_open())
        throw std::runtime_error("Failed to load and then to generate ini file");
    else
    {
        const int size = 512;
        char buffer[size];

        std::stringstream str;

        bool firstAdded = true;

        std::string pathToLibrary;

        while (in.getline(buffer, size))
            if (buffer[0] != '\0' && (buffer[0] != '-' || buffer[1] != '-')) //not comment or empty line
            {
                if (firstAdded)
                {
                    firstAdded = false;
                    pathToLibrary = buffer;
                }
                else
                    addItemToRecordSystem(recordSystem, pathToLibrary, buffer);
            }
    }
}
