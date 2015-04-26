#pragma once

#define FLOORCOUNT 4

#define BUTTONCOUNT 3

/* 	How long should the auction manager wait 
	before it should assign an order */
#define AUCTION_TIME 2

#define DOOR_OPEN_TIME 3

#define ORDER_TIMEOUT 30


typedef enum {
	DIRECTION_DOWN,
	DIRECTION_STOP,
	DIRECTION_UP
} motor_direction_t;

typedef enum {
	BUTTON_CALL_UP,
	BUTTON_CALL_DOWN,
	BUTTON_COMMAND
} button_type_t;
