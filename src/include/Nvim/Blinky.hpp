#ifndef NVIM_BLINKY_HPP
#define NVIM_BLINKY_HPP

#include <Nvim/DataHolder.hpp>
#include <Nvim/GUIOptions.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

/* Blinky is cursor. It is a renderer which obtains info
 * from dataHolder and graphical info from GUIOptions.
 */

namespace nvim
{
    class Blinky
    {
        public:
            enum class Mode {INSERT, NORMAL}; 

        public:
                 Blinky(nvim::DataHolder &dataHolder, nvim::GUIOptions &GUIOptions);

            void update(sf::Time dt, bool layoutChanged = false);
            
            void render(sf::RenderTarget &target, const sf::RenderStates &states) const;

            void changeMode(Mode mode);

            void setTextOffset(int x, int y);

        private:
            nvim::DataHolder     
                &mDataHolder;
            nvim::GUIOptions     
                &mGUIOptions;

            sf::Time 
                 mBlinkPeriod;
            sf::Clock           
                 mTimer;

            sf::RectangleShape  
                 mCursor;

            sf::Text            
                 mDrawableChar;

            bool mIsON;

            Mode mMode;

            bool mModeChanged;

            int  mOffsetX, mOffsetY;
    };
}

#endif
