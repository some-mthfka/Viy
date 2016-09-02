#ifndef RECORD_HPP
#define RECORD_HPP

#include <SFML/System/String.hpp>

#include <Record.hpp>
#include <NamedItem.hpp>
#include <ItemWithPath.hpp>

class Record: public NamedItem, public ItemWithPath
{
    public:
             Record();

        void open();

    private:
        static int
             count;

        bool mSelected;
};

#endif
