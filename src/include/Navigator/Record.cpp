#include <Record.hpp>

#include <Utility.hpp>

int Record::count = 0;

Record::Record()
    : mSelected(false)
{
    setName(sf::String("Record ").toUtf32() + 
                sf::String(toString(Record::count)).toUtf32());
    Record::count++;
}
