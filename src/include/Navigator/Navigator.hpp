#ifndef NAVIGATOR_HPP
#define NAVIGATOR_HPP

#include <RecordSystem.hpp>
#include <NormalNavigatorMode.hpp>
#include <SearchNavigatorMode.hpp>
#include <RenameNavigatorMode.hpp>
#include <Navigator/Signals.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <utility> //pair

class Navigator 
{
    public:
             Navigator(NavigatorSignals &signals);

        void render(sf::RenderTarget &target, const sf::RenderStates &states) const;

        void update(sf::Time dt);

        void handleEvent(const sf::Event &event);

    private:
        void switchToMode(NavigatorMode &mode);

        NavigatorMode::Mode getCurrentMode() const;

    private:
        RecordSystem
             mRecordSystem;

        sf::RenderTexture
             mRenderTexture;

        NavigatorSignals &
             mSignals;

        NavigatorMode::Mode
             mMode;

        NormalNavigatorMode
             mNormalMode;

        SearchNavigatorMode
             mSearchMode;

        RenameNavigatorMode
             mRenameMode;

        NavigatorMode *
             mCurrentMode;
};

#endif
