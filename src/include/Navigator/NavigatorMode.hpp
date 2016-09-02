#ifndef NAVIGATOR_MODE_HPP
#define NAVIGATOR_MODE_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>

#include <RecordSystem.hpp>
#include <Navigator/Signals.hpp>

#include <NavigatorModeRenderer.hpp>

#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.hpp>

#include <utility> //pair

class NavigatorMode 
{
    friend class NavigatorModeRenderer;

    public:
        enum Mode {Normal, Search, Rename};

        enum ItemKind { ParentFolder, CurrentFolder, NestedFolder, NestedRecord };

    public:
             NavigatorMode(RecordSystem &recordSystem, 
                           sf::RenderTexture &renderTexture, 
                           NavigatorSignals &signals);

        void render(sf::RenderTarget &target, const sf::RenderStates &states) const;

        void update(sf::Time dt);

        void refresh() { mNeedUpdate = true; update(sf::Time::Zero); };

        virtual void 
             handleEvent(const sf::Event &event);

        virtual void
             atSwitchingOn();
        virtual void
             atSwitchingOff();

        void resize(int width, int height);

        sf::Vector2u
             getSize() const { return mRenderer.getSize(); }

    private:
        virtual void 
             processTasks() = 0;

        virtual void 
             updateFromRecordSystem() = 0;

        virtual void 
             adjustView();

        void scrollDown();
        void scrollUp();

    protected:
        std::pair<ItemKind, unsigned int>
             getItemInfo(unsigned int index) const;

        virtual void 
             enter();

    protected:
        struct View
        {
            //Item could be: parent folder, nested folder or nested record.
            //Current folder is skipped.
            int topItem = 0,
                currentItem = 0, //relative to the top >= 0
                maxItems = 10,
                totalItems = 0;
        } mView;

        RecordSystem
            &mRecordSystem;

    protected:
        //make const
        Folder * 
             mCurrentFolder;

        std::vector<const Folder *>
             mPathToCurrentFolder;

        bool mNeedUpdate = true;

        NavigatorSignals
            &mSignals;

    private:
        NavigatorModeRenderer
             mRenderer;
};

#endif
