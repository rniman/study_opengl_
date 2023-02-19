#pragma once

#include <map>

#include "command.h"
#include "actor.h"

class InputHandler
{
public:
	std::map<int, Command*> key_command_map;

	void handleInput(Actor& _actor,const  int& _key);
};

void InputHandler::handleInput(Actor& _actor, const int& _key= 0)
{
	for (auto& m : key_command_map)
	{		
		if (m.first == _key)
		{
			m.second->execute(_actor);
		}
	}
	
}