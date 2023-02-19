#pragma once
#include "Game.h"
#include "test_state.h"
#include "test_state_2.h"
#include <iostream>
#include <windows.h>

class TestState : public Game
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


void TestState::enter()
{
	std::cout << "enter" << std::endl;
}

void TestState::exit()
{
	std::cout << "exit" << std::endl;
}

void TestState::pause()
{
}

void TestState::resume()
{
}

void TestState::update(StateMachine* _sm)
{
	std::cout << "time gone" << std::endl;
}

void TestState::handleEvent(StateMachine* _sm)
{
	if (GetAsyncKeyState(VK_SPACE))
	{
		_sm->quit();
	}
}

void TestState::draw()
{
	std::cout << "draw Test" << std::endl;
}
