#include "StateMachine.h"
#include "NetworkListener.h"
#include <iostream>

button_type_t 	lastButton;
int 			lastButtonFloor;

void init_elevator()
{
	ioDriver_setMotorDirection(DIRECTION_UP);
	while (1)
	{
		if (ioDriver_getFloorSensorValue() != -1)
		{
			ioDriver_setMotorDirection(DIRECTION_STOP);
			break;
		}
	}
}

void run_physical()
{
	int newFloor;
	std::cout << "initializing statemachine..\n";
	stateMachine_initialize();
	std::cout << "entering while loop! Press STOP to end the program\n";
	while (1)
	{
		// Check for buttons being pressed
		for (int button = 0; button < 3; button++)
		{
			for (int floor = 0; floor < N_FLOORS; floor++)
			{
				if (((button_type_t)button == BUTTON_CALL_UP) && (floor == 3)) continue;
				if (((button_type_t)button == BUTTON_CALL_DOWN) && (floor == 0)) continue;
				if (ioDriver_isOrderButtonPressed((button_type_t)button, floor))
				{
					if ((lastButton == (button_type_t)button) && (lastButtonFloor == floor)) continue;
					std::cout << "fjgjgjgjgj\n";
					stateMachine_buttonPressed(floor, (button_type_t)button);
					lastButton = (button_type_t)button;
					lastButtonFloor = floor;
				}
			}
		}

		// Check for new floors
		newFloor = ioDriver_getFloorSensorValue();
		if ((newFloor != -1) && (newFloor != getLastFloor()))
			stateMachine_floorReached(newFloor);

		// Check for timeout
		if ((timer_active()) && (timer_done()))
			stateMachine_doorTimeout();

		// Check for order timeout
		Order timedOutOrder = orderManager_checkForOrderTimeout();
		if (timedOutOrder)
			stateMachine_orderTimeOut(timedOutOrder);

		// Stop the program if stop button is pressed
		if (ioDriver_isStopButtonPressed()) {
			ioDriver_setMotorDirection(DIRECTION_STOP);
			break;
		}
	}
}

int main()
{
	std::cout << "Hello and welcome to the elevator program!\n";
	std::cout << "Initalizing driver..\n";
	if (!IoDriver_initialize()) {
		std::cout << "Unable to initialize elevator hardware!\n";
		return 1;
	}

	std::cout << "Initializing elevator..\n";
	init_elevator();
	std::cout << "Going into run physical..\n";

	orderManager_init();	

	networkListener_run();

	run_physical();

	

	return 0;
}
