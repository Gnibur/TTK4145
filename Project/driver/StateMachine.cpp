#include "StateMachine.h"
#include "IoDriver.h"
#include "msgTool.h"
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
#include <pthread.h>


static int					lastFloor;
static motor_direction_t	lastDirection;

typedef enum state {
	IDLE,
	MOVING,
	DOOR_OPEN
} state_t;

static state_t state;


void stateMachine_initialize()
{
	lastFloor		= ioDriver_getFloorSensorValue();
	lastDirection	= DIRECTION_UP;
	auctionManager_init();
}


void stateMachine_eventButtonPressed(int floor, button_type_t button)
{

	if (button == BUTTON_COMMAND) {	
		Order order(button, floor, udp_myIP());
		
		if(orderManager_addOrder(order)) {
			stateMachine_eventNewOrderArrived(order);	
		} else {
			// panic
		}
	}
	else
		auction_start(floor, button);

}

void stateMachine_eventNewOrderArrived(Order order)
{
	switch (state){
	case IDLE: 
		if (order.assignedIP == udp_myIP()){
			if (order.floor == lastFloor){
				orderManager_clearOrder(order);
				msgTool_sendClearOrder(order, orderManager_getOrders(), udp_myIP());
				ioDriver_setDoorOpenLamp();
				timer_start();
				state = DOOR_OPEN;
			} else {
				lastDirection = orderManager_getNextDirection(lastFloor, lastDirection);
				ioDriver_setMotorDirection(lastDirection);	
				state = MOVING;
			}
		}

	case MOVING: 
		break;	
	case DOOR_OPEN:
		if (order.floor == lastFloor){
			orderManager_clearOrder(order);
			timer_start(); 
		}
		break;	
	}
}


void stateMachine_eventFloorReached(int floor)
{
	ioDriver_setFloorIndicator(floor);

	std::cout << "Floor reached: " << floor << std::endl;
	lastFloor = floor;

	switch (state) {
	case MOVING: {

		if (orderManager_shouldStopHere(floor, lastDirection))
		{	
			ioDriver_setMotorDirection(DIRECTION_STOP);
			
			//clearMyOrdersAt(floor);

			OrderList ordersToClear = orderManager_getOrdersOnFloor(floor);
	
			for (auto it = ordersToClear.begin(); it != ordersToClear.end(); ++it) 
			{
				orderManager_clearOrder(*it);		
				msgTool_sendClearOrder(*it, orderManager_getOrders(), udp_myIP());
			}

			ioDriver_setDoorOpenLamp();
			timer_start();

			state = DOOR_OPEN;
		}
		// FAILSAFE: STOP THE ELEVATOR AT BOUNDARY FLOORS ANYWAY.
		else if (floor == 0 || floor == (N_FLOORS - 1)) {
			ioDriver_setMotorDirection(DIRECTION_STOP);
			state = IDLE;
		}
		break;
	}
		
	case IDLE: case DOOR_OPEN:
		std::cout << "FloorReached should not be called when idle or door open\n";
	}	
}


void stateMachine_eventDoorTimedOut()
{
	switch (state) {
	case DOOR_OPEN: {
		timer_reset();
		ioDriver_clearDoorOpenLamp();

		motor_direction_t nextDirection = orderManager_getNextDirection(lastFloor, lastDirection);
		ioDriver_setMotorDirection(nextDirection);	

		if (nextDirection != DIRECTION_STOP) {
			state = MOVING;
			lastDirection = nextDirection;			
		 } else
			state = IDLE;
		break;
	}	
	case MOVING: case IDLE:
		std::cout << "Door timeout should not happen door is not open\n";
	}
}

void stateMachine_eventOrderTimedOut(Order order)
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
		stateMachine_eventButtonPressed(order.floor, order.direction);
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
