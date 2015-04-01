#include "StateMachine.h"
#include <ctime>

StateMachine::StateMachine()
{
    //First: Create your elevator
    myElevator.elevID = 1;          // TODO: Find some way to generate random ID.
    myElevator.currentFloor = -1;   // Start at unknown floor
    myElevator.dir = DIRECTION_UP;  // Start by going up.
    myElevator.goalOrder = {-1, DIRECTION_UNDEFINED, 0};
    
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
    time_t timer;
    while (1)
    {
        int floor = getFloorSensorValue();
        
        /******************************************/
        /*------------ STATEMACHINE --------------*/
        /******************************************/
        
        // *** STATE: INIT *** //
        if (myState == INIT)
        {
            // Trigger: Floor being reached
            if (floor != -1)
            {
                setMotorDirection(DIRECTION_STOP);
                myState = AT_FLOOR;
            }
        }
        // ------------------ //
        
        // *** STATE: AT_FLOOR *** //
        else if (myState == AT_FLOOR)
        {
            // Trigger: Floor was just reached
            if (myElevator.goalOrder.floor == floor)
            {
                time_start = time(&timer);
                time_duration = 3;
                myState = DOOR_OPEN;
            }
            
            // Trigger: Door was just closed
            if (myElevator.goalOrder.floor == -1)
            {
                Order newOrder = myManager.getOrderWithLowestCost(floor, (order_direction_t)myElevator.dir);
                // Case 1: Cannot find new order? Go to IDLE
                if (newOrder.floor == -1)
                {
                    time_start = time(&timer);
                    time_duration = 5;
                    myState = IDLE;
                }
                // Case 2: Can find new order? Go to DRIVING
                else
                {
                    myElevator.goalOrder = newOrder;
                    myElevator.driveHere(newOrder.floor);
                    myState = DRIVING;
                } 
            }
        }
        // ------------------ //
        
        // *** STATE: DOOR_OPEN *** //
        else if (myState == DOOR_OPEN)
        {
            if (time_start + time_duration <= time(&timer))
            {
                // TODO: Close door.
                floorReached();
                myElevator.goalOrder.floor = -1;
                myState = AT_FLOOR;
            }
        }
        // ----------------------- //
        
        // *** STATE: DRIVING *** //
        else if (myState == DRIVING)
        {
            if (floor == myElevator.goalOrder.floor)
            {
                setMotorDirection(DIRECTION_STOP);
                myState = AT_FLOOR;
            }
        }
        // --------------------- //
        
        // *** STATE: IDLE *** //
        else
        {
            if (time_start + time_duration <= time(&timer))
            {
                myState = AT_FLOOR;
            }
        }
        // ------------------- //
        
        
        /******************************************/
        /*-------- CHECK FOR BUTTONS -------------*/
        /******************************************/
        for (int i = 0; i < N_FLOORS; i++)
        {
            if (isOrderButtonPressed(BUTTON_CALL_UP, i)) newOrder(i, DIRECTION_UP);
            if (isOrderButtonPressed(BUTTON_CALL_DOWN, i)) newOrder(i, DIRECTION_DOWN);
            if (isOrderButtonPressed(BUTTON_COMMAND, i)) newOrder(i, DIRECTION_STOP);
        }
        
        /******************************************/
        /*----- CHECK FOR NEW PASSED FLOORS ------*/
        /******************************************/
        if ((floor != -1) && (floor != myElevator.currentFloor))
        {
            // TODO: Set light
        }
    }
}

int main () {
    StateMachine testMachine;
    testMachine.run();
	return 0;
}
