#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "OrderManager.h"
//#include "IoDriver.h"

typedef enum {
  UP = 0,
  INSIDE,
  DOWN
} button_t;


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
  	void eventFloorReached(int floor, motor_direction_t direction);
  	void eventDoorTimeout();
};

#endif
