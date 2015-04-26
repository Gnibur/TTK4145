#include "StateMachine.h"
#include "NetworkListener.h"
#include "IoDriver.h"
#include "DoorTimer.h"
#include "udp.h"
#include "OrderManager.h"
#include "BackupWatcher.h"
#include <unistd.h>

#include <iostream>
#include <assert.h>

void checkForEvents();
void checkButtonsFromEvents();

int main()
{
	std::cout << "Heisprogram har startet\n";
	
	FSM_initialize();

	startBackupProcess();

	assert(networkListener_run() == true);

	orderManager_recover();

	checkForEvents();
}

void checkForEvents()
{
	int oldFloorReading = ioDriver_getFloorSensorValue();
	while (1)
	{
		usleep(10000);
		checkButtonsFromEvents();

		int newFloorReading = ioDriver_getFloorSensorValue();
		if (oldFloorReading != newFloorReading){
			oldFloorReading = newFloorReading;
			if (newFloorReading >= 0 && newFloorReading < FLOORCOUNT)	
				FSM_handleFloorReached(newFloorReading);
			else if (newFloorReading == -1)
				FSM_handleFloorLeft();
		}
			

		if ((doortimer_isActive()) && (doortimer_isDone()))
			FSM_handleDoorTimedOut();

		Order timedOutOrder = orderManager_checkForOrderTimeout();
		if (timedOutOrder.isValid())
			FSM_handleOrderTimedOut(timedOutOrder);

	}
}

void checkButtonsFromEvents()
{
	static int oldButtonStates[BUTTONCOUNT][FLOORCOUNT] = {{0}};
	for (int button = BUTTON_CALL_UP; button <= BUTTON_COMMAND; button++)
	{
		for (int floor = 0; floor < FLOORCOUNT; floor++)
		{
			if ( (button == BUTTON_CALL_UP) && (floor == FLOORCOUNT - 1) ) 
				continue;
			if ( (button == BUTTON_CALL_DOWN) && (floor == 0) ) 
				continue;

			bool newState  = ioDriver_isOrderButtonPressed((button_type_t)button, floor);
			if (newState != oldButtonStates[button][floor]){
				oldButtonStates[button][floor] = newState;
				if (newState == true)
					FSM_handleButtonPressed(floor, (button_type_t)button);
			}
		}
	}

}

