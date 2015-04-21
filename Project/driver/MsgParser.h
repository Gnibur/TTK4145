#pragma once

#include "Enums.h"
#include "DataStructures.h"

#include <string>


using std::string;

enum MsgType {
	NEW_ORDER_MSG,
	CLEAR_ORDER_MSG,
	ORDER_LIST_MSG,
	ORDER_COST_REQUEST,
	ORDER_COST_REPLY,
	INVALID_MESSAGE
};

/* Should this be defined here? */
struct Offer {
	int cost;
	int floor;
	button_t direction;
	string fromIP; // necessary?
};



class MsgParser {
public:
	MsgType   getMessageType(string message);
	Order     getOrderFromMessage(string message);
	OrderList getOrderListFromMessage(string message);
	Order     getOrderCostRequestFromMessage(string message);
	Offer     getOrderCostReplyFromMessage(string message);

	string    makeNewOrderMsg(Order order, OrderList updatedOrderList);
	string    makeClearOrderMsg(Order order, OrderList updatedOrderList);
	string    makeOrderListMsg(OrderList orderList);
	string    makeOrderCostRequestMsg(int floor, button_t direction);
	string    makeOrderCostReplyMsg(int cost, int floor, button_t dir, string IP);

private:

};