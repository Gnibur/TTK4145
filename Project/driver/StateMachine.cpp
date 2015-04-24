#include "StateMachine.h"
#include "IoDriver.h"
#include "MsgParser.h"
#include "AuctionManager.h"
#include "OrderManager.h"
#include "udp.h"
#include "Timer.h"
#include <algorithm>
#include <string>
#include <cstring>
#include <cmath>
#include <ctime>
#include <iostream>
#include <unistd.h>

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

	if (button == BUTTON_COMMAND)
	{	
		Order order(button, floor, getMyIP(), -1);
		if(orderManager_newOrder(order))
		{
        	//usleep(10000);
			stateMachine_newOrder(order);
		}
	}
	else
	{
		auction_start(floor, button);
	}
}

void stateMachine_newOrder(Order order)
{
	switch (state){
	case IDLE:
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
	std::cout << "Floor reached: " << floor << std::endl;
	lastFloor = floor;

	ioDriver_setFloorIndicator(floor);
	OrderList ordersToClear = orderManager_getOrdersOnFloor(floor);

	if (!ordersToClear.empty() && stateMachine_shouldIStopHere(ordersToClear))
	{
		ioDriver_setMotorDirection(DIRECTION_STOP);

		for (auto it = ordersToClear.begin(); it != ordersToClear.end(); ++it) 
		{
			orderManager_clearOrder(*it);
			//usleep(10000);
		}

		ioDriver_setDoorOpenLamp();
		timer_start();

		state = DOOROPEN;
	}
	// FAILSAFE: STOP THE ELEVATOR AT BOUNDARY FLOORS ANYWAY.
	else
	{
		if ((floor == 0) || (floor == N_FLOORS - 1))
		{
			ioDriver_setMotorDirection(DIRECTION_STOP);
			state = IDLE;
		}
	}
}

bool stateMachine_shouldIStopHere(OrderList ordersOnFloor)
{
    button_type_t buttonDirection = (lastDirection == DIRECTION_DOWN)? BUTTON_CALL_DOWN : BUTTON_CALL_UP;
    for (auto it = ordersOnFloor.begin(); it != ordersOnFloor.end(); ++it)
    {
        if ((buttonDirection == it->direction) || (it->direction == BUTTON_COMMAND) || (lastDirection == DIRECTION_STOP))
            return true;
    }
    if (orderManager_getNextDirection(lastFloor, lastDirection) != lastDirection)
        return true;
        
    return false;
}

void stateMachine_doorTimeout()
{
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
			ioDriver_setMotorDirection(nextDirection);
			lastDirection = nextDirection;
			state = MOVING;
		}
		else
		{
			state = IDLE;
			lastDirection = DIRECTION_STOP;
		}
	}
	else
	{
		stateMachine_floorReached(ioDriver_getFloorSensorValue());
	}
}

void stateMachine_orderTimeOut(Order order)
{
	if (order.direction != BUTTON_COMMAND)
	{
		std::cout << "----------------------\n";
		std::cout << "WARNING WARNING WARNING\n";
		std::cout << "ORDER TIMED OUT\n";
		std::cout << "IP: " << order.assignedIP << " FLOOR: " << order.floor << " DIRECTION: " << order.direction << "\n";
		std::cout << "----------------------\n";
		orderManager_clearOrder(order);


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
