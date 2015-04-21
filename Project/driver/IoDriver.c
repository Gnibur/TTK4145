#include "channels.h"
#include "IoDriver.h"
//#include "io.h"
#include "io_physical.h"

#include <assert.h>
#include <stdlib.h>

// Number of signals and lamps on a per-floor basis (excl sensor)
#define N_BUTTONS 3

static const int lamp_channel_matrix[N_FLOORS][N_BUTTONS] = {
    {LIGHT_UP1, LIGHT_DOWN1, LIGHT_COMMAND1},
    {LIGHT_UP2, LIGHT_DOWN2, LIGHT_COMMAND2},
    {LIGHT_UP3, LIGHT_DOWN3, LIGHT_COMMAND3},
    {LIGHT_UP4, LIGHT_DOWN4, LIGHT_COMMAND4},
};


static const int button_channel_matrix[N_FLOORS][N_BUTTONS] = {
    {BUTTON_UP1, BUTTON_DOWN1, BUTTON_COMMAND1},
    {BUTTON_UP2, BUTTON_DOWN2, BUTTON_COMMAND2},
    {BUTTON_UP3, BUTTON_DOWN3, BUTTON_COMMAND3},
    {BUTTON_UP4, BUTTON_DOWN4, BUTTON_COMMAND4},
};

bool IoDriver_initialize(void) {
    int i;

    // Init hardware
    if (!io_init(ET_comedi))
        return false;

    for (i = 0; i < N_FLOORS; ++i) {
        if (i != 0)
	  clearOrderButtonLamp(BUTTON_CALL_DOWN, i);

        if (i != N_FLOORS - 1)
	  clearOrderButtonLamp(BUTTON_CALL_UP, i);

        clearOrderButtonLamp(BUTTON_COMMAND, i);
    }

    // Clear stop lamp, door open lamp, and set floor indicator to ground floor.
    clearStopLamp();
    clearDoorOpenLamp();
    setFloorIndicator(0);

    // Return success.
    return true;
}

void ioDriver_setMotorDirection(motor_direction_t direction) {
  switch (direction) {
  case DIRECTION_DOWN:
    io_set_bit(MOTORDIR);
    io_write_analog(MOTOR, 2800);
    break;
  case DIRECTION_STOP:
    io_write_analog(MOTOR, 0);
    break;
  case DIRECTION_UP:
    io_clear_bit(MOTORDIR);
    io_write_analog(MOTOR, 2800);
    break;
  }
}

void ioDriver_setDoorOpenLamp(void) {
  io_set_bit(LIGHT_DOOR_OPEN);
}
void ioDriver_clearDoorOpenLamp(void){
  io_clear_bit(LIGHT_DOOR_OPEN);
}


bool ioDriver_isElevatorObstructed(void) {
    return io_read_bit(OBSTRUCTION);
}

bool ioDriver_isStopButtonPressed(void) {
    return io_read_bit(STOP);
}

void ioDriver_setStopLamp(void) {
  io_set_bit(LIGHT_STOP);
}
void ioDriver_clearStopLamp(void){
  io_clear_bit(LIGHT_STOP);
}

int ioDriver_getFloorSensorValue(void) {
    if (io_read_bit(SENSOR_FLOOR1))
        return 0;
    else if (io_read_bit(SENSOR_FLOOR2))
        return 1;
    else if (io_read_bit(SENSOR_FLOOR3))
        return 2;
    else if (io_read_bit(SENSOR_FLOOR4))
        return 3;
    else
        return -1;
}

void ioDriver_setFloorIndicator(int floor) {
    assert(floor >= 0);
    assert(floor < N_FLOORS);

    // Binary encoding. One light must always be on.
    if (floor & 0x02)
        io_set_bit(LIGHT_FLOOR_IND1);
    else
        io_clear_bit(LIGHT_FLOOR_IND1);

    if (floor & 0x01)
        io_set_bit(LIGHT_FLOOR_IND2);
    else
        io_clear_bit(LIGHT_FLOOR_IND2);
}

bool ioDriver_isOrderButtonPressed(button_type_t type, int floor) {
    assert(floor >= 0);
    assert(floor < N_FLOORS);
    assert(!(type == BUTTON_CALL_UP && floor == N_FLOORS - 1));
    assert(!(type == BUTTON_CALL_DOWN && floor == 0));
    assert(type == BUTTON_CALL_UP || type == BUTTON_CALL_DOWN || type == BUTTON_COMMAND);

    if (io_read_bit(button_channel_matrix[floor][type]))
        return 1;
    else
        return 0;
}


void ioDriver_setOrderButtonLamp(button_type_t type, int floor){
    assert(floor >= 0);
    assert(floor < N_FLOORS);
    assert(!(type == BUTTON_CALL_UP && floor == N_FLOORS - 1));
    assert(!(type == BUTTON_CALL_DOWN && floor == 0));
    assert(type == BUTTON_CALL_UP || type == BUTTON_CALL_DOWN || type == BUTTON_COMMAND);


    io_set_bit(lamp_channel_matrix[floor][type]);
}

void ioDriver_clearOrderButtonLamp(button_type_t type, int floor){
    assert(floor >= 0);
    assert(floor < N_FLOORS);
    assert(!(type == BUTTON_CALL_UP && floor == N_FLOORS - 1));
    assert(!(type == BUTTON_CALL_DOWN && floor == 0));
    assert(type == BUTTON_CALL_UP || type == BUTTON_CALL_DOWN || type == BUTTON_COMMAND);

    io_clear_bit(lamp_channel_matrix[floor][type]);
}
