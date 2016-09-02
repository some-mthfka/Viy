#ifndef NORMAL_NAVIGATOR_MODE_HPP
#define NORMAL_NAVIGATOR_MODE_HPP

#include <NavigatorMode.hpp>
#include <RecordSystem.hpp>

class NormalNavigatorMode: public NavigatorMode
{
    public:
             NormalNavigatorMode(RecordSystem &recordSystem, 
                                 sf::RenderTexture &renderTexture,
                                 NavigatorSignals &signals);

        virtual void 
             processTasks() override;

        virtual void 
             updateFromRecordSystem() override;

        virtual void 
             handleEvent(const sf::Event &event);

    private:
        struct
        {
            bool addFolder = false,
                 addRecord = false;
        } mTasks;
};

#endif
