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
	// TODO: Send out the update!
	
	// Then: Check for new goal
	Order newOrder = myManager.getOrderWithLowestCost(myElevator.currentFloor, myElevator.dir);
	if (!(newOrder == myElevator.goalOrder)) 
	{
		myElevator.goalOrder = newOrder;
		myElevator.driveHere(newOrder.floor);
	}
}

void StateMachine::floorReached()
{
    // TODO: Stop the elevator for 3 secs maybe.
    // First: Update the list
    myManager.updateList(order_status::DELETE, myManager.getList(), myElevator.goalOrder);
    
    // Then: Check for new goal
    Order newOrder = myManager.getOrderWithLowestCost(myElevator.currentFloor, myElevator.dir);
    myElevator.goalOrder = newOrder;
    myElevator.driveHere(newOrder.floor);
}

void StateMachine::orderButtonPressed()
{
    // TODO: Send message about button being pressed!
    
}

int main () {
	return 0;
}
