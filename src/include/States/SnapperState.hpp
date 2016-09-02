#ifndef SNAPPER_STATE_HPP
#define SNAPPER_STATE_HPP

#include <Backbone/State.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class SnapperState: public State
{
    public:
             SnapperState(StateStack &stack, State::Context context);

        void		
             draw(sf::RenderTarget &target, const sf::RenderStates &states);

        virtual bool        
             update(sf::Time dt) override;

        virtual bool        
             handleEvent(const sf::Event &event) override;

        void captureScreen();

    private:
        void resize(int width, int height);

        sf::IntRect 
             getSelectionRect() const;

        void adjustSelectionSprite();

        bool isSelectionActive() const;

    private:
        struct
        {
            sf::Vector2i
                 pointA, pointB, dragPoint;

            bool isDragging = false,
                 isMouseButtonPressed = false;

            void move(sf::Vector2i v)
            {
                pointA += v;
                pointB += v;
            }

        } mSelection;

        const int SelectionBorderWidth = 2;

    private:
        sf::Image
             mScreenshot;

        sf::Texture
             mTexture;

        sf::Sprite
             mSprite,
             mSelectionSprite;

        sf::RectangleShape
             mSelectionBorder;
};

#endif // STAB_STATE_HPP
