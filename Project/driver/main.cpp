#include "StateMachine.h"

void run_physical()
{
	int newFloor;
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
	}
}

int main()
{
	return 0;
}