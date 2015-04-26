#pragma once

#include "OrderManager.h"
#include "AuctionManager.h"

#include <string>

/* 	MsgParser is a utility library for parsing and building messages.
	The messages are xml-formatted. */



using std::string;

enum MsgType {
	NEW_ORDER_MSG,
	CLEAR_ORDER_MSG,
	ORDER_LIST_MSG,
	ORDER_COST_REQUEST,
	ORDER_COST_REPLY
};

string msgParser_makeNewOrderMsg(	Order order, OrderList updatedOrderList, string fromIP);
string msgParser_makeClearOrderMsg(	Order order, OrderList updatedOrderList, string fromIP);
string msgParser_makeOrderListMsg(	OrderList orderList, string fromIP);

string msgParser_makeOrderCostRequestMsg(int floor, button_type_t direction, string fromIP);
string msgParser_makeOrderCostReplyMsg(Offer offer, std::string fromIP);

bool msgParser_getMessageType(string message, MsgType *msgType);
bool msgParser_getSenderIP(string message, string *senderIp);
bool msgParser_getOrderFromMessage(string message, Order *order);
bool msgParser_getOrderListFromMessage(string message, OrderList *orderList);
bool msgParser_getOrderCostRequestFromMessage(string message, int *floor, button_type_t *direction);
bool msgParser_getOfferFromMessage(string message, Offer *offer);



