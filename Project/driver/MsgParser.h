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


MsgType   msgParser_getMessageType(string message);
Order     msgParser_getOrderFromMessage(string message);
OrderList msgParser_getOrderListFromMessage(string message);
Order     msgParser_getOrderCostRequestFromMessage(string message);
Offer     msgParser_getOfferFromMessage(string message);

string    msgParser_makeNewOrderMsg(Order order, OrderList updatedOrderList);
string    msgParser_makeClearOrderMsg(Order order, OrderList updatedOrderList);
string    msgParser_makeOrderListMsg(OrderList orderList);
string    msgParser_makeOrderCostRequestMsg(int floor, button_type_t direction);
string    msgParser_makeOrderCostReplyMsg(Offer offer);


