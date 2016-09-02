#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.hpp>
#include <StateStack.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>


class Application
{
	public:
		     Application();
		void run();
		

	private:
		void processInput();
		void update(sf::Time dt);
		void render();

        void switchToWindow(sf::RenderWindow &window);
        void switchWindow();

		void updateStatistics(sf::Time dt);
		void registerStates();

	private:
		static const sf::Time	
             TimePerFrame;

		sf::RenderWindow		
             mRegularWindow, 
             mFullscreenWindow,
            *mCurrentWindow,
            *mLastWindow;  //used to restore old window type when exiting snapper state on signal

        bool mFullscreen = false,
             mSnapperIsOff = true;

		TextureHolder			
             mTextures;

	  	FontHolder
             mFonts;

        State::Signals          
             mStateSignals;

		StateStack				
             mStateStack;

		sf::Text				
             mStatisticsText;

		sf::Time				
             mStatisticsUpdateTime;

		std::size_t				
             mStatisticsNumFrames;
};

#endif // APPLICATION_HPP
