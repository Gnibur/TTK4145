#include "StateMachine.h"

StateMachine::StateMachine()
{
}			// Init function

void StateMachine::newOrder(int floor, order_direction_t direction)
{
	// First: Update the list
	int elevator;
	if (direction == DIRECTION_UNDEFINED) elevator = 0;
	else elevator = myElevator.elevID;
	Order order = {floor, direction, elevator};
	myManager.updateList(order_status::NEW, myManager.getList(), order);
	
	// Then: Check for new goal
	Order newOrder = myManager.getOrderWithLowestCost(myElevator.currentFloor, myElevator.dir);
	if (!(newOrder == myElevator.goalOrder)) 
	{
		myElevator.goalOrder = newOrder;
		driveHere(newOrder.floor);
	}
}

void StateMachine::floorReached()
{
}

void StateMachine::orderButtonPressed()
{
}

int main () {
	return 0;
}
