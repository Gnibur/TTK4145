#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "OrderManager.h"
//#include "IoDriver.h"

typedef enum {
  UP = 0,
  DOWN,
  INSIDE
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
  void eventNewOrder(button_type_t button);
  void eventFloorReached(int floor);
  void eventDoorTimeout();
};

#endif