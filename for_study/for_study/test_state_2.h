#pragma once
#include "Game.h"
#include "test_state.h"
#include <iostream>
#include <windows.h>

class Test2State : public Game
{
public:
	virtual void enter() override;
	virtual void exit() override;
	virtual void pause() override;
	virtual void resume() override;
	virtual void update(StateMachine* _sm) override;
	virtual void handleEvent(StateMachine* _sm) override;
	virtual void draw() override;
};


void Test2State::enter()
{
	std::cout << "enter" << std::endl;
}

void Test2State::exit()
{
	std::cout << "exit" << std::endl;
}

void Test2State::pause()
{
}

void Test2State::resume()
{
}

void Test2State::update(StateMachine* _sm)
{
	std::cout << "time gone" << std::endl;
}

void Test2State::handleEvent(StateMachine* _sm)
{
	if (GetAsyncKeyState(VK_SPACE))
	{
		_sm->quit();
	}
}

void Test2State::draw()
{
	std::cout << "draw Test2" << std::endl;
}
