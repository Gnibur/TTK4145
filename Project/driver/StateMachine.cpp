#include "StateMachine.h"
#include "IoDriver.h"
#include <algorithm>
#include <string>
#include <cmath>
#include <ctime>

int					lastFloor;
motor_direction_t	lastDirection;

void stateMachine_buttonPressed(int floor, button_type_t button)
{
	ioDriver_setOrderButtonLamp(button, floor);
	if (button == BUTTON_COMMAND)
	{
		time_t timer;
		int assignedDate = time(&timer);
		Order order = {button, floor, IP, assignedDate }; 
		orderManager_newOrder(order);
		std::string newOrderMsg = makeOrderListMsg(NEW_ORDER, orderManager.getOrders(), order);
		udp_send(BROADCAST_PORT, newOrderMsg, MAXLENGTH_BUF);
	}
	else
	{
		budmanager.start(button, floor);
	}
}

void stateMachine_floorReached(int floor, motor_direction_t direction)
{
	ioDriver_setFloorIndicator(floor)
	lastFloor = floor;

	button_type_t buttonDirection			= (button_type_t)direction;
	button_type_t buttonOppositeDirection	= (button_type_t)abs((int)direction - 2);

	OrderList ordersInDirection			= orderManager_getOrdersOnFloorInDirection(floor, buttonDirection);
	OrderList ordersInOppositeDirection = orderManager_getOrdersOnFloorInDirection(floor, buttonOppositeDirection);
	OrderList ordersToClear;

	bool stop = false;

	if (!ordersInDirection.empty())
	{
		ordersToClear = ordersInDirection;
		stop = true;
	}
	else
	{
		if ((ordersInOppositeDirection.empty()) && (orderManager_getNextDirection(floor, direction) != direction))
		{
			ordersToClear = ordersInOppositeDirection;
			stop = true;
		}
	}
	if (stop == true)
	{
		ioDriver_setMotorDirection(DIRECTION_STOP);
		for (auto it = ordersToClear.begin(); it != ordersToClear.end(); ++it) 
		{
			orderManager_clearOrder(*it);
			std::string clearOrderMsg;
			udp_send(BROADCAST_PORT, clearOrderMsg, MAXLENGTH_BUF);
			// open door, set light, start timer
		}
	}
}

void stateMachine_doorTimeout()
{
	clearDoorOpenLamp();
	// state is IDLE
	motor_direction_t nextDirection = orderManager_getNextDirection(state.lastFloor, state.lastDirection);
	setMotorDirection(nextDirection);
	lastDirection = nextDirection;
}

void stateMachine_orderTimeOut(Order order)
{
	if (order.direction != BUTTON_COMMAND)
	{
		orderManager_clearOrder(order);
		std::string clearOrderMsg;
		udp_send(BROADCOAST_PORT, clearOrderMsg, MAXLENGTH_BUF);

		// TODO: Needs failsafe method, so the elevator doesn't die here and everything is lost..
		ioDriver_eventButtonPressed(order.floor, order.direction);
	}
}
