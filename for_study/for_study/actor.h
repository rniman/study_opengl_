#pragma once
class Actor
{
public:
	virtual void moveFoward() = 0;
	virtual void moveBack() = 0;
	virtual void moveRight() = 0;
	virtual void moveLeft() = 0;
};