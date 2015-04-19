#include "StateMachine.h"
//#include "IoDriver.h"
#include <algorithm>
#include <string>

void StateMachine::eventButtonPressed(button button)
{
	orderManager.newOrder(button.floor, (order_direction_t)button.direction);
	//setOrderButtonLamp(button.direction, button.floor);
	//budmanager.start(button);
}


void StateMachine::eventFloorReached(int reachedFloor, motor_direction_t direction)
{

	//setFloorIndicator(reachedFloor)
	state.lastFloor = reachedFloor;

  // Ordermanager is also responsible for the light
	if (shouldIStopHere(reachedFloor, direction)){
		OrderList ordersToClear = orderManager.findOrdersOnFloor(reachedFloor, direction);
		orderManager.clearOrders(ordersToClear);

		//string clearOrdersMsg = makeOrderListMsg(CLEAR_ORDER, orderManager.getGlobalList(), ordersToClear);
		// send out order info to network. Also when internal?

		// set door open light and start timer
	}

}

void StateMachine::eventDoorTimeout()
{
	// clear door light;
	// state is IDLE
	//motor_direction_t nextDirection = orderManager.getNextDirection(currentFloor, lastDirection);
	//motor.setDirection(nextDirection);

	//state.lastDirection = nextDirection;
}

void StateMachine::orderTimeOut(Order order)
{
	//eventButtonPressed();
}

bool StateMachine::shouldIStopHere(int floor, motor_direction_t direction)
{
	if (!orderManager.hasOrderOnFloor(floor)) 
		return false;
	OrderList ordersOnFloor = orderManager.findOrdersOnFloor(floor, direction);
	if (!ordersOnFloor.empty())
	{
		return true;
	}
	else
	{
		int directionalAdder;
		int limit;
		if (direction == DIRECTION_UP)
		{
			directionalAdder = 1;
			limit = N_FLOORS - 1;
		}
		else
		{
			directionalAdder = -1;
			limit = 0;
		}
		for (int i = floor; i != limit; i += directionalAdder)
		{
			if (orderManager.hasOrderOnFloor(i)) 
				return false;
		}
	}
	return true;
}
