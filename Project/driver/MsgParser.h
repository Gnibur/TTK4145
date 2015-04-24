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
	ORDER_COST_REPLY
};


bool msgParser_getMessageType(string message, MsgType *msgType);
bool msgParser_getSenderIP(string message, std::string *senderIp);
bool msgParser_getOrderFromMessage(string message, Order *order);
bool msgParser_getOrderListFromMessage(string message, OrderList *orderList);
bool msgParser_getOrderCostRequestFromMessage(string message, int *floor, button_type_t *direction);
bool msgParser_getOfferFromMessage(string message, Offer *offer);

string    msgParser_makeNewOrderMsg(Order order, OrderList updatedOrderList);
string    msgParser_makeClearOrderMsg(Order order, OrderList updatedOrderList);
string    msgParser_makeOrderListMsg(OrderList orderList);
string    msgParser_makeOrderCostRequestMsg(int floor, button_type_t direction);
string    msgParser_makeOrderCostReplyMsg(Offer offer);


