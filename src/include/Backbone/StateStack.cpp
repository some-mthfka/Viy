#include <StateStack.hpp>

#include <cassert>


StateStack::StateStack(State::Context context)
: mStack()
, mPendingList()
, mContext(context)
, mFactories()
{
}

void StateStack::update(sf::Time dt)
{
	// Iterate from top to bottom, stop as soon as update() returns false
    /*
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}
    */

    if (!mStack.empty())
        mStack.back() -> update(dt);

	applyPendingChanges();
}

void StateStack::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
	// Draw all active states from bottom to top
	//for(State::Ptr& state: mStack)
		//state->draw(target, states);

    if (!mStack.empty())
        mStack.back() -> draw(target, states);
}

void StateStack::handleEvent(const sf::Event& event)
{
	// Iterate from top to bottom, stop as soon as handleEvent() returns false
	//for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	//{
		//if (!(*itr)->handleEvent(event))
			//break;
	//}

    if (!mStack.empty())
        mStack.back() -> handleEvent(event);

	applyPendingChanges();
}

void StateStack::pushState(States::ID stateID)
{
	mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
	mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
	return mStack.empty();
}

State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());

	return found->second();
}

void StateStack::applyPendingChanges()
{
	for(PendingChange change: mPendingList)
	{
		switch (change.message)
		{
			case Push:
				mStack.push_back(createState(change.stateID));
				break;

			case Pop:
				mStack.pop_back();
				break;

			case Clear:
				mStack.clear();
				break;
		}
	}

	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Message message, States::ID stateID)
    : message(message)
    , stateID(stateID)
{
}
