#include "StateMachine.h"


StateMachine::StateMachine()
{
    //First: Create your elevator
    myElevator.elevID = 1;          // TODO: Find some way to generate random ID.
    myElevator.currentFloor = -1;   // Start at unknown floor
    myElevator.dir = DIRECTION_UP;  // Start by going up.
    
    //Second: Define your state
    myState = INIT;
    
    //Third: Make and ordermanager
    
}

void StateMachine::newOrder(int floor, motor_direction_t direction)
{
	// First: Update the list
	int elevator;
	if (direction == (motor_direction_t)DIRECTION_UNDEFINED) elevator = 0;
	else elevator = myElevator.elevID;
	Order order = {floor, (order_direction_t)direction, elevator};
	myManager.updateList(NEW, myManager.getList(), order);
	// TODO: Send out the update!
	
	// Then: Check for new goal
	Order newOrder = myManager.getOrderWithLowestCost(myElevator.currentFloor, (order_direction_t)myElevator.dir);
	if ((!(newOrder == myElevator.goalOrder)) && ((myState == DRIVING) || (myState == IDLE))) //Ooops! This is ugly. TODO: Make this pretty.
	{
		myElevator.goalOrder = newOrder;
		myElevator.driveHere(newOrder.floor);
	}
}

void StateMachine::floorReached()
{
    // First: Update the list
    myManager.updateList(DELETE, myManager.getList(), myElevator.goalOrder);
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
    Order order = myManager.getOrderWithLowestCost(myElevator.currentFloor, (order_direction_t)myElevator.dir);
    if (order.floor != -1) // TODO: Make sure you have sufficient operators for this.
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
        if ((floor != -1) && ((myState == DRIVING) || (myState == INIT)))
        {
            if (floor != myElevator.currentFloor)
            {
                myElevator.currentFloor = floor;
                // TODO: Set light.
                if (myState == INIT)
                {
                    setMotorDirection(DIRECTION_STOP);
                    myState = AT_FLOOR;
                }
            }
            if (floor == myElevator.goalOrder.floor) 
            {
                floorReached();
                myState = AT_FLOOR;
            }
        }
        if (timeOut() && ((myState == AT_FLOOR) || (myState == IDLE)))
        {
            handleTimeout();
        }
        for (int i = 0; i < N_FLOORS; i++)
        {
            if (isOrderButtonPressed(BUTTON_CALL_UP, i)) newOrder(i, DIRECTION_UP);
            if (isOrderButtonPressed(BUTTON_CALL_DOWN, i)) newOrder(i, DIRECTION_DOWN);
            if (isOrderButtonPressed(BUTTON_COMMAND, i)) newOrder(i, DIRECTION_STOP);
        }
        if (myState == INIT)
        {
            setMotorDirection(myElevator.dir);
        }
    }
}

int main () {
    StateMachine testMachine;
    testMachine.run();
	return 0;
}
