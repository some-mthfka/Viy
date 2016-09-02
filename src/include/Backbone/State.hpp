#ifndef STATE_HPP
#define STATE_HPP

#include <StateIdentifiers.hpp>
#include <ResourceIdentifiers.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <memory>


namespace sf
{
	class RenderWindow;
}

class StateStack;
class Player;

class State
{
	public:
		typedef std::unique_ptr<State> Ptr;

        struct Signals
        {
            struct 
            {
                bool status = false;
            } enterFullscreen;

            struct 
            {
                bool status = false;
            } restoreScreenState; //that is previous screen state

            struct 
            {
                bool status = false;
            } switchToSnapper;

            struct 
            {
                bool status = false;
            } switchWindowType;
        };

		struct Context
		{
                 Context(sf::RenderWindow& window, 
                         TextureHolder& textures, 
                         FontHolder& fonts, 
                         Signals &signals);

			sf::RenderWindow	
                *window;
			TextureHolder	
                *textures;
			FontHolder
                *fonts;
            Signals
                *signals;
		};

	public:
							State(StateStack& stack, Context context);
		virtual				~State();

		virtual void		draw(sf::RenderTarget &target, const sf::RenderStates &states) = 0;
		virtual bool		update(sf::Time dt) = 0;
		virtual bool		handleEvent(const sf::Event& event) = 0;


	protected:
		void				requestStackPush(States::ID stateID);
		void				requestStackPop();
		void				requestStateClear();

		Context				getContext() const;


	private:
		StateStack*			mStack;
		Context				mContext;
};

#endif // STATE_HPP
