#pragma once

#include "OrderManager.h"
#include "GlobalDefines.h"

/* 	The state machine is responsible for handling events.
	It controls the motor to the desired floors. */

void FSM_doSafeStop(int);

void FSM_initialize();

void FSM_handleButtonPressed(int floor, button_type_t button);
void FSM_handleFloorReached(int floor);

void FSM_handleAuctionStarted(int floor, button_type_t button);
void FSM_handleNewOrderArrived(Order order);

void FSM_handleDoorTimedOut();

// methods which are added for failsafety
void FSM_handleOrderTimedOut(Order order);
void FSM_handleFloorLeft();


