#include "StateMachine.h"
#include "IoDriver.h"
#include <algorithm>

void StateMachine::eventButtonPressed(button button)
{

  orderManager.newOrder(button.floor, (order_direction_t)button.direction);
  setOrderButtonLamp(button.direction, button.floor);
  budmanager.start(button);
}


void StateMachine::eventFloorReached(int reachedFloor, motor_direction_t direction)
{

  setFloorIndicator(reachedFloor)
  state.lastFloor = reachedFloor;

  // ordermanager is also responsible for the light
  if (orderManager.hasOrderOnFloor(floor)){
      orderManager.clearOrdersOnFloor(floor, direction);

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