#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "OrderManager.h"
#include "IoDriver.h"

typedef enum {
	INIT = 0,
	IDLE = 1,
	AT_FLOOR = 2,
	DRIVING = 3
} elevator_state_t;

/*------------------------------------------------------*/
/*---------------- ELEVATOR STRUCT ---------------------*/
/*------------------------------------------------------*/
typedef struct {
	int elevID;				// From 1 to n, unique for this elevator
	int currentFloor;
	int goalFloor; 			// The elevators desired goal
	motor_direction_t dir;	// The elevators current or last used dir
	elevator_state_t elevState;
} elevator_t;


/*------------------------------------------------------*/
/*------------------ STATE MACHINE ---------------------*/
/*------------------------------------------------------*/
class StateMachine {
public:
	StateMachine();			// Init function
	newOrder(int floor);	// Send update to others and find new order
	floorReached();			// Send update to others and find new order
	orderButtonPressed();	// Send msg about changing the light
};

#endif