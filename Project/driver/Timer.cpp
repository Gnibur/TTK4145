#include "Timer.h"
#include <ctime>

int		time_start;
bool	time_active;

void timer_start() {
	time_t timer;
	
	time_start		= time(&timer);
	time_active	= true;
}

bool timer_done() {
	time_t timer;
	int time_now = time(&timer);
	if ((time_now - TIMER_DOOR) > time_start)
		return true;
	else
		return false;
}

void timer_reset() {
	time_start		= -1;
	time_active		= false;
}

bool timer_active()
{
	return time_active;
}
