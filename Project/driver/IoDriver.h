#ifndef IO_DRIVER_H
#define IO_DRIVER_H

#include <stdbool.h>

#include "GlobalDefines.h"


/**
  Initialize elevator.
  @return Non-zero on success, 0 on failure.
*/
bool IoDriver_initialize(void);


/**
  Sets the motor direction of the elevator.
  @param direction New direction of the elevator.
*/
void ioDriver_setMotorDirection(motor_direction_t direction);

void ioDriver_setDoorOpenLamp(void);
void ioDriver_clearDoorOpenLamp(void);


/**
  Get signal from obstruction switch.
  @return 1 if obstruction is enabled. 0 if not.
*/
bool ioDriver_isElevatorObstructed(void);

bool ioDriver_isStopButtonPressed(void);

void ioDriver_setStopLamp(void);
void ioDriver_clearStopLamp(void);

/**
  Get floor sensor signal.
  @return -1 if elevator is not on a floor. 0-3 if elevator is on floor. 0 is
    ground floor, 3 is top floor.
*/
int ioDriver_getFloorSensorValue(void);



/**
  Set floor indicator lamp for a given floor.
  @param floor Which floor lamp to turn on. Other floor lamps are turned off.
*/
void ioDriver_setFloorIndicator(int floor);



/**
  Button types for function elev_set_button_lamp() and elev_get_button().
*/



/**
  Gets a button signal.
  @param button Which button type to check. Can be BUTTON_CALL_UP,
    BUTTON_CALL_DOWN or BUTTON_COMMAND (button "inside the elevator).
  @param floor Which floor to check button. Must be 0-3.
  @return 0 if button is not pushed. 1 if button is pushed.
*/
bool ioDriver_isOrderButtonPressed(button_type_t type, int floor);



/**
  Set a button lamp.
  @param lamp Which type of lamp to set. Can be BUTTON_CALL_UP,
    BUTTON_CALL_DOWN or BUTTON_COMMAND (button "inside" the elevator).
  @param floor Floor of lamp to set. Must be 0-3
  @param value Non-zero value turns lamp on, 0 turns lamp off.
*/

void ioDriver_setOrderButtonLamp(button_type_t type, int floor);
void ioDriver_clearOrderButtonLamp(button_type_t type, int floor);


#endif // #ifndef IO_DRIVER_H
