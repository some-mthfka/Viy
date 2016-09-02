#include <SnapperState.hpp>

#include <Screenshot.h>
#include <Utility.hpp>

#include <iostream>
#include <fstream>
#include <ctime>

#include <Debug.hpp>

SnapperState::SnapperState(StateStack& stack, State::Context context)
    : State(stack, context)
{
    captureScreen();

    getContext().signals -> enterFullscreen.status = true;

    mSelectionBorder.setFillColor(sf::Color::White);
}

void SnapperState::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
    target.draw(mSprite);
    target.draw(mSelectionBorder);
    target.draw(mSelectionSprite, sf::BlendNone);
}

bool SnapperState::update(sf::Time dt)
{
	return true;
}

void SnapperState::resize(int width, int height)
{

}

void SnapperState::captureScreen()
{
    takeScreenshot(mScreenshot);
    mTexture.loadFromImage(mScreenshot);
    mSprite.setTexture(mTexture, /*resetRect =*/ true);
    mSprite.setColor( {96, 96, 96, 255} );
}

bool SnapperState::isSelectionActive() const
{
    return getSelectionRect().width != 0 && getSelectionRect().height != 0;
}

void SnapperState::adjustSelectionSprite()
{
    auto selectionRect = getSelectionRect();

    mSelectionSprite.setTexture(mTexture);
    mSelectionSprite.setTextureRect(selectionRect);
    mSelectionSprite.setPosition(selectionRect.left, selectionRect.top);

    if (isSelectionActive())
    {
        mSelectionBorder.setSize( {static_cast<float>(selectionRect.width) + 2 * SelectionBorderWidth, 
                                   static_cast<float>(selectionRect.height) + 2 * SelectionBorderWidth} );

        mSelectionBorder.setPosition(mSelectionSprite.getPosition() - 
                                     sf::Vector2f(SelectionBorderWidth, SelectionBorderWidth));

        mSelectionBorder.setFillColor(sf::Color::White);
    }
    else
        mSelectionBorder.setSize( {0, 0} );
}

sf::IntRect SnapperState::getSelectionRect() const
{
    sf::IntRect rect;

    rect.left = std::min(mSelection.pointA.x, mSelection.pointB.x);
    rect.top = std::min(mSelection.pointA.y, mSelection.pointB.y);

    auto right = std::max(mSelection.pointA.x, mSelection.pointB.x);
    auto bottom = std::max(mSelection.pointA.y, mSelection.pointB.y);

    rect.width = right - rect.left;
    rect.height = bottom - rect.top;

    return rect; 
}

bool SnapperState::handleEvent(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::Closed:
            exit(true);
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Return:
                    if (isSelectionActive())
                    {
                        std::string imagename = toString(time(NULL)) + ".png";

                        sf::Image saved;
                        saved.create(getSelectionRect().width, getSelectionRect().height);
                        saved.copy(mScreenshot, 0, 0, getSelectionRect());
                        saved.saveToFile("library/images/" + imagename);

                        //open the images file and add entry to it
                        std::ofstream out("library/images/ImageList.txt", std::ios::app);
                        out << "<|" + imagename + "|10|0|>" << std::endl << std::endl;

                        requestStackPop();

                        getContext().signals -> restoreScreenState.status = true;
                    }
                    break;

                case sf::Keyboard::Escape:
                    if (isSelectionActive())
                    {
                        mSelection.pointA = mSelection.pointB;
                        mSelection.isDragging = false;
                        adjustSelectionSprite();
                    }
                    else
                    {
                        requestStackPop();
                        getContext().signals -> restoreScreenState.status = true;
                    }

                    break;

                default:
                    break;
            }
            //if (event.key.code == sf::Keyboard::I && !mFocusedOnNvim)
                //mFocusedOnNvim = true;
            break;

        case sf::Event::MouseButtonPressed:
            {
                auto mousePosition = sf::Mouse::getPosition();

                mSelection.isMouseButtonPressed = true;

                if (getSelectionRect().contains(mousePosition))
                {
                    mSelection.isDragging = true;
                    mSelection.dragPoint = mousePosition;
                }
                else
                {
                    mSelection.pointA = mSelection.pointB = mousePosition;
                    adjustSelectionSprite();
                }
            }

            break;

        case sf::Event::MouseMoved:
            if (mSelection.isMouseButtonPressed)
            {
                auto mousePosition = sf::Mouse::getPosition();

                if (mSelection.isDragging)
                {
                    mSelection.move(mousePosition - mSelection.dragPoint);
                    mSelection.dragPoint = mousePosition;
                }
                else
                    mSelection.pointB = mousePosition;

                adjustSelectionSprite();
            }

            break;

        case sf::Event::MouseButtonReleased:
            mSelection.isDragging = false;
            mSelection.isMouseButtonPressed = false;
            break;

        default:
            break;
    }

	return true;
}
