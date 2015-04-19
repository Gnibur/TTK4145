#ifndef ENUMS_H
#define ENUMS_H

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

typedef enum {
	ORDER_DOWN,
	ORDER_INSIDE,
	ORDER_UP
} order_direction_t;

typedef enum {
	UP = 0,
	INSIDE,
	DOWN
} button_t;

typedef enum {
	ADD_ORDER,
	CLEAR_ORDER,
	UPDATE_LIST
} list_update_type;

#endif