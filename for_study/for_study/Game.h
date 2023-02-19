#pragma once
#include <stack>

class Game;


class StateMachine
{
public:
	std::stack<Game*> state;
	bool running = true;

	void pushState(Game* _state);
	void popState();
	void changeState(Game* _state);
	void quit();
	void run(Game* _start_state);
};

class Game
{
public:

	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual void update(StateMachine* _sm) = 0;
	virtual void handleEvent(StateMachine* _sm) = 0;
	virtual void draw() = 0;
};

void StateMachine::pushState(Game* _state)
{
	if (!state.empty())
		state.top()->pause();

	state.push(_state);
	_state->enter();
}

void StateMachine::popState()
{
	if (!state.empty())
	{
		state.top()->exit();
		state.pop();
	}
	
	if (!state.empty())
		state.top()->resume();
}

void StateMachine::changeState(Game* _state)
{
	if (!state.empty())
	{
		state.top()->exit();
		state.pop();
	}
	state.push(_state);
	_state->enter();
}

void StateMachine::quit()
{
	running = false;
}

void StateMachine::run(Game* _start_state)
{
	state.push(_start_state);
	state.top()->enter();

	while (running)
	{
		state.top()->handleEvent(this);
		state.top()->update(this);
		state.top()->draw();
	}

	while (!state.empty())
	{
		state.top()->exit();
		state.pop();
	}
}