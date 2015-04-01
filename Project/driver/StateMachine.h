#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include "IoDriver.h"
#include "OrderManager.h"

typedef enum {
	INIT = 0,
	IDLE = 1,
	AT_FLOOR = 2,
	DRIVING = 3,
	DOOR_OPEN = 4
} elevator_state_t;

/*------------------------------------------------------*/
/*---------------- ELEVATOR STRUCT ---------------------*/
/*------------------------------------------------------*/
// TODO: Ugly variables. Change pls.
typedef struct {
	int elevID;				// From 1 to n, unique for this elevator
	int currentFloor;
	Order goalOrder; 		// The elevators desired goal
	motor_direction_t dir;	// The elevators current or last used dir
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
	elevator_state_t myState;
	int time_start;
	int time_duration;
public:
	StateMachine();											// Init function
	void newOrder(int floor, motor_direction_t direction);	// Send update to others and find new order
	void floorReached();									// Send update to others and find new order
	void orderButtonPressed();								// Send msg about changing the light
	bool timeOut();											// TODO: Make this!
	void handleTimeout();
	void run();												// Runs the entire process
};

#endif
