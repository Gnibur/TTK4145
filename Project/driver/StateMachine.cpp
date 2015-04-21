#include "StateMachine.h"
#include "IoDriver.h"
#include "MsgParser.h"
#include <algorithm>
#include <string>
#include <cmath>
#include <ctime>
#include <iostream>

int					lastFloor;
motor_direction_t	lastDirection;
std::string testIP	= "";

void stateMachine_initialize()
{
	lastFloor		= ioDriver_getFloorSensorValue();
	lastDirection	= DIRECTION_STOP;
}

void stateMachine_buttonPressed(int floor, button_type_t button)
{
	ioDriver_setOrderButtonLamp(button, floor);
	if (button == BUTTON_COMMAND)
	{
		time_t timer;
		int assignedDate = time(&timer);
		Order order(button, floor, testIP, assignedDate);
		orderManager_newOrder(order);
		std::string newOrderMsg = MsgParser::makeNewOrderMsg(order, orderManager_getOrders());
		udp_send(BROADCAST_PORT, newOrderMsg.c_str(), MAXLENGTH_BUF);
	}
	else
	{
		auction_start(floor, button);
		// Elevator should probably add own bid
	}
}

void stateMachine_floorReached(int floor)
{
	lastFloor = floor;
	ioDriver_setFloorIndicator(floor);

	button_type_t buttonDirection			= (lastDirection == DIRECTION_UP)? BUTTON_CALL_UP : BUTTON_CALL_DOWN;
	button_type_t buttonOppositeDirection	= (lastDirection == DIRECTION_UP)? BUTTON_CALL_DOWN : BUTTON_CALL_UP;

	OrderList ordersInDirection			= orderManager_getOrdersOnFloorInDirection(floor, buttonDirection);
	OrderList ordersInOppositeDirection = orderManager_getOrdersOnFloorInDirection(floor, buttonOppositeDirection);
	OrderList ordersToClear;

	bool stop = false;

	if (!ordersInDirection.empty())
	{
		std::cout << "Orders in direction is not empty!\n";
		ordersToClear = ordersInDirection;
		stop = true;
	}
	else
	{
		std::cout << "Orders in opposite direction?\n";
		if ((!ordersInOppositeDirection.empty()) && (orderManager_getNextDirection(floor, lastDirection) != lastDirection))
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
			std::cout << "Entering orders to clear...\n";
			std::cout << "FLOOR: " << it->floor << std::endl;
			orderManager_clearOrder(*it);
			std::string clearOrderMsg = MsgParser::makeClearOrderMsg(order, orderManager_getOrders());
			udp_send(BROADCAST_PORT, clearOrderMsg.c_str(), MAXLENGTH_BUF);
			ioDriver_clearOrderButtonLamp(it->direction, it->floor);
			ioDriver_setDoorOpenLamp();
		}
		timer_start();
	}
}

void stateMachine_doorTimeout()
{
	std::cout << "Entering doortimeout...\n";
	timer_reset();
	ioDriver_clearDoorOpenLamp();
	motor_direction_t nextDirection = orderManager_getNextDirection(lastFloor, lastDirection);
	ioDriver_setMotorDirection(nextDirection);
	lastDirection = nextDirection;
}

void stateMachine_orderTimeOut(Order order)
{
	if (order.direction != BUTTON_COMMAND)
	{
		orderManager_clearOrder(order);
		std::string clearOrderMsg = MsgParser::makeClearOrderMsg(order, orderManager_getOrders());
		udp_send(BROADCAST_PORT, clearOrderMsg.c_str(), MAXLENGTH_BUF);

		// TODO: Needs failsafe method, so the elevator doesn't die here and everything is lost..
		stateMachine_buttonPressed(order.floor, order.direction);
	}
}

int getLastFloor()
{
	return lastFloor;
}
motor_direction_t getLastDirection()
{
	return lastDirection;
}
