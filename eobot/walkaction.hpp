// Supress unused variable warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"


#ifndef WALK_ACTION_HPP_INCLUDED
#define WALK_ACTION_HPP_INCLUDED
#include "eodata.hpp"
#include "npc.hpp"

class WalkAction{
public:
	Direction walkDirection;
	unsigned int delay;

	WalkAction(Direction walkDirection, int delay = 0) : walkDirection(walkDirection), delay(delay) {};
};

#endif // WALK_ACTION_HPP_INCLUDED
