#include "StateMachine.h"
#include "IoDriver.h"
#include "MsgParser.h"
#include "AuctionManager.h"
#include <algorithm>
#include <string>
#include <cstring>
#include <cmath>
#include <ctime>
#include <iostream>

int					lastFloor;
motor_direction_t	lastDirection;

typedef enum state {
	IDLE,
	MOVING,
	DOOROPEN
} state_t;

static state_t state;

void stateMachine_initialize()
{
	lastFloor		= ioDriver_getFloorSensorValue();
	lastDirection	= DIRECTION_STOP;
	auctionManager_init();
}

void stateMachine_buttonPressed(int floor, button_type_t button)
{
	std::cout << "Entering buttonPressed \n";

	if (button == BUTTON_COMMAND)
	{

		Order order(button, floor, getMyIP(), -1);
		orderManager_newOrder(order);
		std::string newOrderMsg = msgParser_makeNewOrderMsg(order, orderManager_getOrders());
		udp_send(BROADCAST_PORT, newOrderMsg.c_str(), strlen(newOrderMsg.c_str()));

		stateMachine_newOrder(floor, button);
	}
	else
	{
		std::cout << "Auction start... \n";
		auction_start(floor, button);

		// Elevator should probably add own bid
	}
}

void stateMachine_newOrder(int floor, button_type_t button)
{
	ioDriver_setOrderButtonLamp(button, floor);
	switch (state){
	case IDLE:
		std::cout << "newOrder in state idle\n";
		stateMachine_updateDirection();
		break;
	case DOOROPEN:
		break;
	case MOVING:
		break;
	}	
	
}

void stateMachine_floorReached(int floor)
{
	std::cout << "Floor: " << floor << std::endl;
	lastFloor = floor;

	ioDriver_setFloorIndicator(floor);
	OrderList ordersToClear = orderManager_getOrdersOnFloor(floor);

	if (!ordersToClear.empty())
	{
		std::cout << "I AM NOT EMPTY GOD DAMNIT!\n";
		ioDriver_setMotorDirection(DIRECTION_STOP);

		for (auto it = ordersToClear.begin(); it != ordersToClear.end(); ++it) 
			{
			std::cout << "Entering orders to clear...\n";
			std::cout << "FLOOR: " << it->floor << std::endl;

			orderManager_clearOrder(*it);
			std::string clearOrderMsg = msgParser_makeClearOrderMsg(*it, orderManager_getOrders());

			udp_send(BROADCAST_PORT, clearOrderMsg.c_str(), strlen(clearOrderMsg.c_str()));
			ioDriver_clearOrderButtonLamp(it->direction, it->floor);
			
		}

		ioDriver_setDoorOpenLamp();
		timer_start();

		state = DOOROPEN;
	}
}

void stateMachine_doorTimeout()
{
	std::cout << "Entering doortimeout...\n";
	timer_reset();
	ioDriver_clearDoorOpenLamp();

	stateMachine_updateDirection();
}

void stateMachine_updateDirection()
{
	OrderList ordersOnFloor = orderManager_getOrdersOnFloor(ioDriver_getFloorSensorValue());
	if (ordersOnFloor.empty())
	{
		motor_direction_t nextDirection = orderManager_getNextDirection(lastFloor, lastDirection);
		if (nextDirection != DIRECTION_STOP)
		{
			std::cout << "Lets start moving!\n";
			ioDriver_setMotorDirection(nextDirection);
			lastDirection = nextDirection;
			state = MOVING;
		}
		else
		{
			std::cout << "Go back to idle.\n";
			state = IDLE;
		}
	}
	else
	{
		std::cout << "Floor reached.\n";
		stateMachine_floorReached(ioDriver_getFloorSensorValue());
	}
}

void stateMachine_orderTimeOut(Order order)
{
	/*if (order.direction != BUTTON_COMMAND)
	{
		orderManager_clearOrder(order);
		std::string clearOrderMsg = msgParser_makeClearOrderMsg(order, orderManager_getOrders());
		udp_send(BROADCAST_PORT, clearOrderMsg.c_str(), strlen(clearOrderMsg.c_str()));

		// TODO: Needs failsafe method, so the elevator doesn't die here and everything is lost..
		//stateMachine_buttonPressed(order.floor, order.direction);
	}*/
}

int getLastFloor()
{
	return lastFloor;
}
motor_direction_t getLastDirection()
{
	return lastDirection;
}
