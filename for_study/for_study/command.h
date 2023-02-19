#pragma once


class Command
{
public:
	//virtual ~Command();
	//virtual void execute(Actor& _actor, const float& _time) = 0;
	virtual void execute(Actor& _actor) = 0;
};

class FowardCommand : public Command
{
public:

	void execute(Actor& _actor) override
	{
		_actor.moveFoward();
	}
};

class BackCommand : public Command
{
public:

	void execute(Actor& _actor) override
	{
		_actor.moveBack();
	}
};

class RightCommand : public Command
{
public:

	void execute(Actor& _actor) override
	{
		_actor.moveRight();
	}
};

class LeftCommand : public Command
{
public:

	void execute(Actor& _actor) override
	{
		_actor.moveLeft();
	}
};