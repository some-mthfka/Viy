#ifndef NAVIGATOR_SIGNALS_HPP
#define NAVIGATOR_SIGNALS_HPP

struct NavigatorSignals
{
    //Used for signalling to navigator that a switch to
    //normal mode is needed on entering a folder.
    //Needed for when in search mode.
    struct
    {
        bool status = false;
    } switchToNormal;

    struct
    {
        bool status = false;
        Record *record;
    } enterRecord;

    struct
    {
        bool status = false;
    } quit;
};

#endif
