#include "Timer.h"
#include "GlobalDefines.h"
#include <ctime>

static time_t	timeAtStart;
static bool		isActive;

void doortimer_start() {
	timeAtStart	= time(0);
	isActive	= true;
}

bool doortimer_isDone() {
	if (isActive) {
		time_t time_now = time(0);
		if (timeAtStart + DOOR_OPEN_TIME > time_now)
			return true;
		else
			return false;
	} else 
		return false;
}

void doortimer_reset() {
	timeAtStart		= -1;
	isActive		= false;
}

bool doortimer_isActive()
{
	return isActive;
}
