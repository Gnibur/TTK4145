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
	if ((!(newOrder == myElevator.goalOrder)) && ((myState == DRIVING) || (myState == IDLE))) //Ooops! This is ugly. TODO: Make this pretty.
	{
		myElevator.goalOrder = newOrder;
		myElevator.driveHere(newOrder.floor);
	}
}

void StateMachine::floorReached()
{
    // First: Update the list
    myManager.updateList(order_status::DELETE, myManager.getList(), myElevator.goalOrder);
    // TODO: Set start-time, and when to timeout.
}

void StateMachine::orderButtonPressed()
{
    // TODO: Send message about button being pressed!
    
}

bool StateMachine::timeOut() 
{
    return true;
}
void StateMachine::handleTimeout()
{
    Order order = myManager.getOrderWithLowestCost(myElevator.currentFloor, myElevator.dir);
    if (order) // TODO: Make sure you have sufficient operators for this.
    {
        myElevator.goalOrder = order;
        myElevator.driveHere(order.floor);
    }
}

void StateMachine::run()
{
    while (1)
    {
        // TRIGGER 1: Check for floors being passed
        int floor = getFloorSensorValue();
        if ((floor != -1)) && (myState = DRIVING))
        {
            if (floor != myElevator.currentFloor)
            {
                myElevator.currentFloor = floor;
                // TODO: Set light.
            }
            if (floor == myElevator.goalOrder.floor) 
            {
                floorReached();
                myState = AT_FLOOR;
            }
        }
        if (timeOut()) && ((myState == AT_FLOOR) || (myState == IDLE)))
        {
            handleTimout();
        }
        for (int i = 0; i < N_FLOORS; i++)
        {
            if (isOrderButtonPressed(button_type_t::BUTTON_CALL_UP, i) newOrder(i, DIRECTION_UP);
            if (isOrderButtonPressed(button_type_t::BUTTON_CALL_DOWN, i) newOrder(i, DIRECTION_DOWN);
            if (isOrderButtonPressed(button_type_t::BUTTON_COMMAND, i) newOrder(i, DIRECTION_UNDEFINED);
        }
    }
}

int main () {
	return 0;
}
