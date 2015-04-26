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
#include <assert.h>


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
	assert(IoDriver_initialize());

	ioDriver_setMotorDirection(DIRECTION_UP);
	
	while (ioDriver_getFloorSensorValue() == -1)
		;
	ioDriver_setMotorDirection(DIRECTION_STOP);
	lastFloor		= ioDriver_getFloorSensorValue();
	ioDriver_setFloorIndicator(lastFloor);
	lastDirection	= DIRECTION_UP;
	state 			= IDLE;

	auctionManager_init();
}


void stateMachine_eventButtonPressed(int floor, button_type_t button)
{		
	assert(floor >= 0 && floor < N_FLOORS);

	if (button == BUTTON_COMMAND) {	
		Order order(button, floor, udp_myIP());
		
		if(orderManager_addOrder(order, SEND_UPDATE)) {
			stateMachine_eventNewOrderArrived(order);	
		} else {
			// panic
		}
	}
	else {
		auction_start(floor, button);

		int cost = orderManager_getCost(lastFloor, floor, lastDirection, button);
		Offer offer(cost, floor, button, udp_myIP());
		auction_addBid(offer);
	}

}

void stateMachine_eventNewOrderArrived(Order order)
{
	assert(order.isValid());

	switch (state){
	case IDLE: 
		if (order.assignedIP == udp_myIP()){
			if (order.floor == lastFloor){
				if (orderManager_clearOrder(order, SEND_UPDATE)){
					ioDriver_setDoorOpenLamp();
					timer_start();
					state = DOOR_OPEN;				
				}
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
			if (orderManager_clearOrder(order, SEND_UPDATE))
				timer_start(); 
		}
		break;	
	}
}


void stateMachine_eventAuctionStarted(int floor, button_type_t button)
{
	assert(floor >= 0 && floor < N_FLOORS);
	
	int cost = orderManager_getCost(lastFloor, floor, lastDirection, button);
	Offer offer(cost, floor, button, udp_myIP());
	msgTool_sendOrderCostReply(offer, udp_myIP());
}


void stateMachine_eventFloorReached(int floor)
{
	assert(state == MOVING);
	assert(floor >= 0 && floor < N_FLOORS);

	if (lastDirection == DIRECTION_UP)
		assert(floor == lastFloor + 1);
	else if (lastDirection == DIRECTION_DOWN)
		assert(floor == lastFloor - 1); 		

	ioDriver_setFloorIndicator(floor);

	std::cout << "Floor reached: " << floor << std::endl;
	lastFloor = floor;



	if (orderManager_shouldStopHere(floor, lastDirection))
	{	
		ioDriver_setMotorDirection(DIRECTION_STOP);
		
		if (orderManager_clearOrdersAt(floor, udp_myIP(), SEND_UPDATE) == true){
			ioDriver_setDoorOpenLamp();
			timer_start();
			state = DOOR_OPEN;
		}
		else {
			std::cout << "Failed to clear orders\n";		
			state = IDLE;

		}
	}
	// FAILSAFE: STOP THE ELEVATOR AT BOUNDARY FLOORS ANYWAY.
	else if (floor == 0 || floor == (N_FLOORS - 1)) {
		ioDriver_setMotorDirection(DIRECTION_STOP);
		state = IDLE;
	}
	

}

void stateMachine_eventFloorLeft()
{
	assert(state == MOVING);
}


void stateMachine_eventDoorTimedOut()
{
	assert(state == DOOR_OPEN);

	timer_reset();
	ioDriver_clearDoorOpenLamp();

	motor_direction_t nextDirection = orderManager_getNextDirection(lastFloor, lastDirection);
	ioDriver_setMotorDirection(nextDirection);	

	if (nextDirection != DIRECTION_STOP) {
		state = MOVING;
		lastDirection = nextDirection;			
	 } else
		state = IDLE;
}

void stateMachine_eventOrderTimedOut(Order order)
{
	assert(order.isValid());
	
	std::cout << "----------------------\n";
	std::cout << "WARNING WARNING WARNING\n";
	std::cout << "ORDER TIMED OUT\n";
	std::cout << "IP: " << order.assignedIP << " FLOOR: " << order.floor << " DIRECTION: " << order.direction << "\n";
	std::cout << "----------------------\n";
	orderManager_clearOrder(order, SEND_UPDATE);


	// TODO: Needs failsafe method, so the elevator doesn't die here and everything is lost..
	stateMachine_eventButtonPressed(order.floor, order.direction);
}
