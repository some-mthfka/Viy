#ifndef RENAME_NAVIGATOR_MODE_HPP
#define RENAME_NAVIGATOR_MODE_HPP

#include <NavigatorMode.hpp>
#include <RecordSystem.hpp>

class RenameNavigatorMode: public NavigatorMode
{
    public:
             RenameNavigatorMode(RecordSystem &recordSystem, 
                                 sf::RenderTexture &renderTexture,
                                 NavigatorSignals &signals);

        virtual void 
             atSwitchingOn();

        virtual void 
             processTasks() override;

        virtual void 
             updateFromRecordSystem() override;

        virtual void 
             handleEvent(const sf::Event &event);
};

#endif
