#ifndef SEARCH_NAVIGATOR_MODE_HPP
#define SEARCH_NAVIGATOR_MODE_HPP

#include <NavigatorMode.hpp>
#include <RecordSystem.hpp>

class SearchNavigatorMode: public NavigatorMode
{
    public:
             SearchNavigatorMode(RecordSystem &recordSystem, 
                                 sf::RenderTexture &renderTexture,
                                 NavigatorSignals &signals);

        virtual void 
             processTasks() override;

        virtual void 
             updateFromRecordSystem() override;

        virtual void 
             handleEvent(const sf::Event &event) override;

        virtual void 
             atSwitchingOn() override;

        virtual void 
             atSwitchingOff() override;

    private:
        //convert to sf::String, cause unicode is added to it on TextEntered event
        std::basic_string<sf::Uint32>
             mSearchQuery;
};

#endif
