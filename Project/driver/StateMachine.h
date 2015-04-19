#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "OrderManager.h"
#include "DataStructures.h"
//#include "IoDriver.h"


typedef struct {
  int lastFloor;
  motor_direction_t lastDirection;
} ElevatorState;


class StateMachine {
private:
	ElevatorState state;
	OrderManager orderManager;
public:
	void eventButtonPressed(button button);
  	void eventFloorReached(int floor, motor_direction_t direction);
  	void eventDoorTimeout();
  	void orderTimeOut(Order order);
  	
  	bool shouldIStopHere(int floor, motor_direction_t direction); // Is this too complicated?
};

#endif
