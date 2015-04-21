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

/* Should this be defined here?
struct Offer {
	int cost;
	int floor;
	button_t direction;
	string fromIP; // necessary?
};*/



class MsgParser {
public:
	static MsgType   getMessageType(string message);
	static Order     getOrderFromMessage(string message);
	static OrderList getOrderListFromMessage(string message);
	static Order     getOrderCostRequestFromMessage(string message);
	static Offer     getOrderCostReplyFromMessage(string message);

	static string    makeNewOrderMsg(Order order, OrderList updatedOrderList);
	static string    makeClearOrderMsg(Order order, OrderList updatedOrderList);
	static string    makeOrderListMsg(OrderList orderList);
	static string    makeOrderCostRequestMsg(int floor, button_type_t direction);
	static string    makeOrderCostReplyMsg(int cost, int floor, button_type_t dir, string IP);

private:

};
