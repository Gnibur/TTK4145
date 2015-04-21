#include "StateMachine.h"
#include <iostream>

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
	cout << "initializing statemachine..\n";
	stateMachine_initialize();
	cout << "entering while loop! Press STOP to end the program\n";
	while (1)
	{
		// Check for buttons being pressed
		for (button_type_t button = 0; (int)button < 3; button++)
		{
			for (int floor = 0; floor < N_FLOORS; floor++)
			{
				if (ioDriver_isOrderButtonPressed(button, floor))
					stateMachine_buttonPressed(floor, button); // Make sure this happens only once.
			}
		}

		// Check for new floors
		newFloor = ioDriver_getFloorSensorSignal();
		if ((newFloor != -1) && (newFloor != lastFloor))
			stateMachine_floorReached(newFloor)

		// Check for timeout
		if ((timer_active) && (timer_done()))
			stateMachine_doorTimeout();

		// Check for order timeout
		Order timedoutOrder = orderManager_checkForOrderTimeout();
		if (timedOutOrder)
			stateMachine_orderTimeout(timedoutOrder);

		// Stop the program if stop button is pressed
		if (isStopButtonPressed()) {
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
	run_physical();

	return 0;
}