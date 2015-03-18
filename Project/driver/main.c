#include "IoDriver.h"
#include <stdio.h>


int main() {
    // Initialize hardware
    if (!IoDriver_initialize()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    setMotorDirection(DIRECTION_UP);

    while (1) {
        // Change direction when we reach top/bottom floor
        if (getFloorSensorValue() == N_FLOORS - 1) {
            setMotorDirection(DIRECTION_DOWN);
        } else if (getFloorSensorValue() == 0) {
            setMotorDirection(DIRECTION_UP);
        }

        // Stop elevator and exit program if the stop button is pressed
        if (isStopButtonPressed()) {
            setMotorDirection(DIRECTION_STOP);
            break;
        }
    }

    return 0;
}
