#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "OrderManager.h"
//#include "IoDriver.h"

typedef enum {
	INIT = 0,
	IDLE = 1,
	AT_FLOOR = 2,
	DRIVING = 3
} elevator_state_t;

typedef enum { 
    DIRECTION_DOWN,
    DIRECTION_STOP,
    DIRECTION_UP
} motor_direction_t;

void setMotorDirection(motor_direction_t direction) { };

/*------------------------------------------------------*/
/*---------------- ELEVATOR STRUCT ---------------------*/
/*------------------------------------------------------*/
// TODO: Ugly variables. Change pls.
typedef struct {
	int elevID;				// From 1 to n, unique for this elevator
	int currentFloor;
	Order goalOrder; 		// The elevators desired goal
	motor_direction_t dir;	// The elevators current or last used dir TODO: Should be motor_direction_t
	elevator_state_t elevState;
	
	void driveHere(int floor)
	{
    	motor_direction_t wantedDir = ((floor - currentFloor) > 0)? DIRECTION_UP : DIRECTION_DOWN;
    	setMotorDirection(wantedDir);
    	dir = wantedDir;
	}
	
} elevator_t;


/*------------------------------------------------------*/
/*------------------ STATE MACHINE ---------------------*/
/*------------------------------------------------------*/
class StateMachine {
	OrderManager myManager;
	elevator_t myElevator;
public:
	StateMachine();											// Init function
	void newOrder(int floor, order_direction_t direction);	// Send update to others and find new order
	void floorReached();									// Send update to others and find new order
	void orderButtonPressed();								// Send msg about changing the light
	void run();												// Runs the entire process
	void driveHere(int floor);								// TODO: This needs to be placed somewhere smart
};

#endif
