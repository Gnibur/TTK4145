#include "StateMachine.h"
#include "NetworkListener.h"
#include "IoDriver.h"
#include "Timer.h"
#include "udp.h"
#include "OrderManager.h"
#include <iostream>


void checkForEvents();
void checkButtonsFromEvents();

int main()
{
	std::cout << "Hello and welcome to the elevator program!\n";
	std::cout << "Initalizing driver..\n";
	
	if (!IoDriver_initialize()) {
		std::cout << "Unable to initialize elevator hardware!\n";
	}
	stateMachine_initialize();
	orderManager_init();

	udp_initialize(BROADCAST_PORT);
	networkListener_run();

	checkForEvents();


	return 0;
}

void checkForEvents()
{
	while (1)
	{
		checkButtonsFromEvents();

		int newFloorReading = ioDriver_getFloorSensorValue();
		if ((newFloorReading != -1) && (newFloorReading != getLastFloor()))
			stateMachine_eventFloorReached(newFloorReading);

		if ((timer_active()) && (timer_done()))
			stateMachine_eventDoorTimedOut();

		Order timedOutOrder = orderManager_checkForOrderTimeout();
		if (timedOutOrder.isValid())
			stateMachine_eventOrderTimedOut(timedOutOrder);

	}
}

void checkButtonsFromEvents()
{
	static int oldButtonStates[3][N_FLOORS] = {{0}};
	for (int button = BUTTON_CALL_UP; button < BUTTON_COMMAND; button++)
	{
		for (int floor = 0; floor < N_FLOORS; floor++)
		{
			if ( (button == BUTTON_CALL_UP) && (floor == N_FLOORS - 1) ) 
				continue;
			if ( (button == BUTTON_CALL_DOWN) && (floor == 0) ) 
				continue;

			bool newState  = ioDriver_isOrderButtonPressed((button_type_t)button, floor);
			if (newState != oldButtonStates[button][floor]){
				oldButtonStates[button][floor] = newState;
				if (newState == true)
					stateMachine_eventButtonPressed(floor, (button_type_t)button);
			}
		}
	}

}

