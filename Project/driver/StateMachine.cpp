#include "StateMachine.h"
#include "IoDriver.h"
#include "AuctionManager.h"
#include "OrderManager.h"
#include "MsgParser.h"
#include "udp.h"
#include "DoorTimer.h"
#include <algorithm>
#include <string>
#include <cstring>
#include <cmath>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <signal.h>

typedef enum state {
	IDLE,
	MOVING,
	DOOR_OPEN
} state_t;

static state_t state;

static int					lastFloor;
static motor_direction_t	lastDirection;

void FSM_doSafeStop(int signum)
{
	while (ioDriver_getFloorSensorValue() == -1)
		;
	ioDriver_setMotorDirection(DIRECTION_STOP);
	ioDriver_setFloorIndicator(lastFloor);
	exit(0);
}

void FSM_initialize()
{
	assert(IoDriver_initialize());

	assert(signal(SIGINT, FSM_doSafeStop) != SIG_ERR);

	ioDriver_setMotorDirection(DIRECTION_UP);
	
	while (ioDriver_getFloorSensorValue() == -1)
		;
		
	ioDriver_setMotorDirection(DIRECTION_STOP);
	ioDriver_setFloorIndicator(lastFloor);
	lastFloor		= ioDriver_getFloorSensorValue();
	lastDirection	= DIRECTION_UP;
	state 			= IDLE;
}


void FSM_handleButtonPressed(int floor, button_type_t button)
{		
	assert(floor >= 0 && floor < FLOORCOUNT);

	if (button == BUTTON_COMMAND) 
	{	
		Order order(floor, button, getMyIP());
		if (orderManager_addOrder(order, SEND_UPDATE))
			FSM_handleNewOrderArrived(order);	
	}
	else 
	{
		// a global order can be handled by everyone, therefore it is auctioned
		auction_start(floor, button);

		int cost = orderManager_getOrderCost(floor, button, lastFloor, lastDirection);
		Offer offer(cost, floor, button, getMyIP());
		auction_addBid(offer);
	}

}

void FSM_handleNewOrderArrived(Order order)
{
	std::cout << "Arrived at state machine!\n";	
	assert(order.isValid());

	std::cout << "Order arrived: Floor: "	<< order.floor 
				<< ", Dir " << order.direction 
				<< ", IP " << order.assignedIP << std::endl;

	switch (state){
	case IDLE: 
		if (order.assignedIP == getMyIP()){
			if (order.floor == lastFloor){
				orderManager_clearOrder(order, SEND_UPDATE);
				ioDriver_setDoorOpenLamp();
				doortimer_start();
				state = DOOR_OPEN;				
				
			} else {
				lastDirection = orderManager_getNextMotorDirection(lastFloor, lastDirection);
				ioDriver_setMotorDirection(lastDirection);	
				state = MOVING;
			}
		}

	case MOVING: 
		break;	
	case DOOR_OPEN:
		if (order.floor == lastFloor && order.assignedIP == getMyIP()){
			if (orderManager_clearOrder(order, SEND_UPDATE))
				doortimer_start();
		}
		break;	
	}
}


void FSM_handleAuctionStarted(int floor, button_type_t button)
{
	assert(floor >= 0 && floor < FLOORCOUNT);
	
	int cost = orderManager_getOrderCost(floor, button, lastFloor, lastDirection);
	Offer offer(cost, floor, button, getMyIP());

	std::string costReply;
	costReply = msgParser_makeOrderCostReplyMsg(offer, getMyIP());
	udp_send(costReply.c_str(), strlen(costReply.c_str()) + 1);
}

void FSM_handleFloorReached(int floor)
{
	std::cout << "Floor reached: " << floor << std::endl;

	assert(state == MOVING);
	assert(floor >= 0 && floor < FLOORCOUNT);

	if (lastDirection == DIRECTION_UP)
		assert(floor == lastFloor + 1);
	else if (lastDirection == DIRECTION_DOWN)
		assert(floor == lastFloor - 1); 		


	lastFloor = floor;
	ioDriver_setFloorIndicator(floor);
	if (orderManager_shouldElevatorStopHere(floor, lastDirection))
	{	
		ioDriver_setMotorDirection(DIRECTION_STOP);
		if (orderManager_clearOrdersAt(floor, getMyIP(), SEND_UPDATE) == true) {
			ioDriver_setDoorOpenLamp();
			doortimer_start();
			state = DOOR_OPEN;
		}
		else 
			state = IDLE;
	}

	// FAILSAFE: STOP THE ELEVATOR AT BOUNDARY FLOORS ANYWAY.
	else if (floor == 0 || floor == (FLOORCOUNT - 1)) {
		ioDriver_setMotorDirection(DIRECTION_STOP);
		state = IDLE;
	}
}

void FSM_handleDoorTimedOut()
{
	assert(state == DOOR_OPEN);
	
	doortimer_reset();
	ioDriver_clearDoorOpenLamp();

	motor_direction_t nextDirection;
	nextDirection = orderManager_getNextMotorDirection(lastFloor, lastDirection);
	ioDriver_setMotorDirection(nextDirection);	

	if (nextDirection != DIRECTION_STOP) {
		state = MOVING;
		lastDirection = nextDirection;			
	 } else
		state = IDLE;
}

void FSM_handleOrderTimedOut(Order order)
{
	std::cout << "----------------------\n";
	std::cout << "WARNING WARNING WARNING\n";
	std::cout << "ORDER TIMED OUT\n";
	std::cout << "IP: " << order.assignedIP << " FLOOR: " << order.floor << " DIRECTION: " << order.direction << "\n";
	std::cout << "----------------------\n";

	assert(order.isValid());

	
	if (order.direction == BUTTON_COMMAND && order.assignedIP == getMyIP()){
		orderManager_addOrder(order, DONT_SEND_UPDATE);
		FSM_handleNewOrderArrived(order);
	} else {
		auction_start(order.floor, order.direction);
		int cost = orderManager_getOrderCost(order.floor, order.direction, lastFloor, lastDirection);
		Offer offer(cost, order.floor, order.direction, getMyIP());
		auction_addBid(offer);
		orderManager_clearOrder(order, SEND_UPDATE);
	}
		
}


void FSM_handleFloorLeft()
{
	assert(state == MOVING);
}
