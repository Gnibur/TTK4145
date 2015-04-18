#include "StateMachine.h"
#include "IoDriver.h"

void StateMachine::eventButtonPressed(button_type_t button, int floor)
{

  orderManager.newOrder(floor, (order_direction_t)direction);
  setOrderButtonLamp(button, floor);
  budmanager.start(button);
}


void StateMachine::eventFloorReached(int reachedFloor)
{

  // set floor indicator

  state.lastFloor = reachedFloor;

  // ordermanager is also responsible for the light
  if (orderManager.hasOrderOnFloor(floor)){
      orderManager.clearOrder(floor);

      // send out order info to network. Also when internal?

      // set door open light and start timer
  }

}

void StateMachine::eventDoorTimeout()
{
  // clear door light;
  // state is IDLE
  elev_direction_t nextDirection = orderManager.getNextDirection(currentFloor, lastDirection);
  motor.setDirection(nextDirection);

  state.lastDirection = nextDirection
}

void StateMachine::orderTimeOut(order)
{
  eventButtonPressed();
}