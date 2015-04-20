#include "StateMachine.h"
#include "IoDriver.h"
#include <algorithm>
#include <string>
#include <cmath>
#include <ctime>

void StateMachine::eventButtonPressed(button_type_t button, int floor)
{
	orderManager.newOrder(floor, button);
	setOrderButtonLamp(button, floor);
	if (button == BUTTON_COMMAND)
	{
		time_t timer;
		int assignedDate = time(&timer);
		Order order = {button, floor, IP, assignedDate }; 
		orderManager.newOrder(order);
		std::string newOrderMsg = makeOrderListMsg(NEW_ORDER, orderManager.getOrders(), order);
		udp.send(BROADCAST_PORT, newOrderMsg, MAXLENGTH_BUF);
	}
	else
	{
		budmanager.start(button, floor);
	}
}


void StateMachine::eventFloorReached(int reachedFloor, motor_direction_t direction)
{
	setFloorIndicator(reachedFloor)
	state.lastFloor	= reachedFloor;

	button_type_t buttonDirection			= (button_type_t)direction;
	button_type_t buttonOppositeDirection	= (button_type_t)abs((int)direction - 2);

	OrderList ordersInDirection			= getOrdersOnFloorInDirection(reachedFloor, buttonDirection);
	OrderList ordersInOppositeDirection = getOrdersOnFloorInDirection(reachedFloor, buttonOppositeDirection);
	OrderList ordersToClear;

	bool stop = false;

	if (!ordersInDirection.empty())
	{
		ordersToClear = ordersInDirection;
		stop = true;
	}
	else
	{
		if ((ordersInOppositeDirection.empty()) && (orderManager.getNextDirection(reachedFloor, direction) != direction))
		{
			ordersToClear = ordersInOppositeDirection;
			stop = true;
		}
	}
	if (stop == true)
	{
		setMotorDirection(DIRECTION_STOP);
		for (auto it = ordersToClear.begin(); it != ordersToClear.end(); ++it) 
		{
			orderManager.clearOrder(*it);
			std::string clearOrderMsg;
			udp.send(BROADCAST_PORT, clearOrderMsg, MAXLENGTH_BUF);
			// open door, set light, start timer
		}
	}
}

void StateMachine::eventDoorTimeout()
{
	clearDoorOpenLamp();
	// state is IDLE
	motor_direction_t nextDirection = orderManager.getNextDirection(state.lastFloor, state.lastDirection);
	setMotorDirection(nextDirection);
	state.lastDirection = nextDirection;
}

void StateMachine::orderTimeOut(Order order)
{
	// How to:
	// Clear order
	// Send out msg
	// Call eventbuttonpressed.
	//eventButtonPressed();
}
