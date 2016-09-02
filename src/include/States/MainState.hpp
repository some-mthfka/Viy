#ifndef MAIN_STATE_HPP
#define MAIN_STATE_HPP

#include <Navigator.hpp>
#include <Neovim.hpp>
#include <Nvim/Signals.hpp>
#include <Navigator/Signals.hpp>

#include <Backbone/State.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>

#include <memory>

class MainState: public State
{
    public:
             MainState(StateStack &stack, State::Context context);

        void		
             draw(sf::RenderTarget &target, const sf::RenderStates &states);

        virtual bool        
             update(sf::Time dt) override;

        virtual bool        
             handleEvent(const sf::Event &event) override;

    private:
        void resize(int width, int height);

    private:
        struct fontLoader
        {
            fontLoader() { bool b = font.loadFromFile("media/neovim.ttf"); 
                           assert(b); }
            sf::Font font;
        };

    private:
        fontLoader            
             mFontLoader;

        NavigatorSignals
             mNavigatorSignals;

        Navigator
             mNavigator;

        nvim::InitData
             mInitData;

        nvim::Signals
             mNeovimSignals;

        std::unique_ptr<nvim::Neovim>
             mNeovim;

        bool mFocusedOnNvim = true,
             mMonitorScreenshotHotkey = false;
};

#endif
