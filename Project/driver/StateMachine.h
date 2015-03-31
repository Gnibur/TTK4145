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

/*------------------------------------------------------*/
/*---------------- ELEVATOR STRUCT ---------------------*/
/*------------------------------------------------------*/
typedef struct {
	int elevID;				// From 1 to n, unique for this elevator
	int currentFloor;
	Order goalOrder; 		// The elevators desired goal
	order_direction_t dir;	// The elevators current or last used dir
	elevator_state_t elevState;
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
};

#endif
