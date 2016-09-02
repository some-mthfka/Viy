#include <Application.hpp>
#include <Utility.hpp>
#include <State.hpp>
#include <StateIdentifiers.hpp>
#include <MainState.hpp>
#include <SnapperState.hpp>

#include <Debug.hpp>

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
    : mRegularWindow(sf::VideoMode(1024, 600), "Viy", sf::Style::Close | sf::Style::Resize)
    , mFullscreenWindow(sf::VideoMode::getFullscreenModes()[0],
                        "Viy",
                        sf::Style::Fullscreen)
    , mCurrentWindow(&mRegularWindow)
    , mLastWindow(nullptr)
    , mStateStack(State::Context(mRegularWindow, mTextures, mFonts, mStateSignals))
    , mStatisticsText()
    , mStatisticsUpdateTime()
    , mStatisticsNumFrames(0)
{
	//mWindow.setKeyRepeatEnabled(false);
    
    mFullscreenWindow.setVisible(false);

	mFonts.load(Fonts::Main, "Media/Sansation.ttf");

	mStatisticsText.setFont(mFonts.get(Fonts::Main));
    mStatisticsText.setColor(sf::Color::Black);
	mStatisticsText.setPosition(0, 0);
	mStatisticsText.setCharacterSize(10u);

	registerStates();
	mStateStack.pushState(States::Main);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mCurrentWindow -> isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			if (mStateStack.isEmpty())
            {
				mRegularWindow.close();
                mFullscreenWindow.close();
            }
		}

		updateStatistics(dt);
		render();
	}
}

void Application::switchWindow()
{
    if (mFullscreen)
        switchToWindow(mRegularWindow);
    else
        switchToWindow(mFullscreenWindow);
}

void Application::switchToWindow(sf::RenderWindow &window)
{
    mCurrentWindow -> setVisible(false);
    mCurrentWindow = &window;
    mCurrentWindow -> setVisible(true);

    mFullscreen = &window == &mFullscreenWindow;

    sf::Event resized;
    resized.type = sf::Event::Resized;
    resized.size.width = mCurrentWindow -> getSize().x;
    resized.size.height = mCurrentWindow -> getSize().y;

    //ultimately adjusts nvim's grid size
    //to the new dimensions
    mStateStack.handleEvent(resized);
}

void Application::processInput()
{
	sf::Event event;
	while (mCurrentWindow -> pollEvent(event))
	{
        if (event.type == sf::Event::Resized)
        {
            // update the view to the new size of the window
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            mCurrentWindow -> setView(sf::View(visibleArea));
        }

        mStateStack.handleEvent(event);
	}
}

void Application::update(sf::Time dt)
{
    if (mStateSignals.enterFullscreen.status)
    {
        switchToWindow(mFullscreenWindow);
        mStateSignals.enterFullscreen.status = false;
    }

    if (mStateSignals.switchWindowType.status)
    {
        switchWindow();
        mStateSignals.switchWindowType.status = false;
    }

    if (mStateSignals.switchToSnapper.status && mSnapperIsOff)
    {
        LOG(switching to snapper)
        LOG(snapperoff set to false)
        mSnapperIsOff = false;
        mStateStack.pushState(States::Snapper);
        mLastWindow = mCurrentWindow;
        mStateSignals.switchToSnapper.status = false;
    }

    if (mStateSignals.restoreScreenState.status)
    {
        //Snapper sets the signal on its exit, so it is off
        LOG(snapperoff set to true)
        mSnapperIsOff = true;
        mStateSignals.switchToSnapper.status = false;
        switchToWindow(*mLastWindow);
        mStateSignals.restoreScreenState.status = false;
    }

	mStateStack.update(dt);
}

void Application::render()
{
	mCurrentWindow -> clear();

    sf::RenderStates states;

	mStateStack.draw(*mCurrentWindow, states);

	mCurrentWindow -> draw(mStatisticsText);

	mCurrentWindow -> display();
}

void Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS: " + toString(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<MainState>(States::Main);
	mStateStack.registerState<SnapperState>(States::Snapper);
}
