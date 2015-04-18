#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "OrderManager.h"
//#include "IoDriver.h"

typedef enum {
  UP = 0,
  INSIDE,
  DOWN
} button_t;

struct button {
	button_type_t button;
	int floor;
}


/*------------------------------------------------------*/
/*------------------ STATE MACHINE ---------------------*/
/*------------------------------------------------------*/

typedef struct {
  lastFloor;
  lastDirection;
} ElevatorState;


class StateMachine {
private:
  ElevatorState state;
public:
	void eventButtonPressed(button_type_t button, int floor)
  	void eventFloorReached(int floor);
  	void eventDoorTimeout();
};

#endif