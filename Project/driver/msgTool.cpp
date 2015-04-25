#include "msgTool.h"
#include "udp.h"
#include "MsgParser.h"
#include <cstring>
#include <iostream>


void msgTool_sendNewOrder(Order order, OrderList updatedOrderList, std::string fromIP){
	std::string msg;
	msg = msgParser_makeNewOrderMsg(order, updatedOrderList, fromIP);
	udp_send(msg.c_str(), strlen(msg.c_str()) + 1);
	std::cout << "Sent new order\n";
}

void msgTool_sendClearOrder(Order order, OrderList updatedOrderList, std::string fromIP){
	std::string msg;
	msg = msgParser_makeClearOrderMsg(order, updatedOrderList, fromIP);
	udp_send(msg.c_str(), strlen(msg.c_str()) + 1);
	std::cout << "Sent clear order\n";
}

void msgTool_sendOrderList(OrderList orderList, std::string fromIP){
	std::string msg;
	msg = msgParser_makeOrderListMsg(orderList, fromIP);
	udp_send(msg.c_str(), strlen(msg.c_str()) + 1);
}

void msgTool_sendOrderCostRequest(int floor, button_type_t direction, std::string fromIP){
	std::string msg;
	msg = msgParser_makeOrderCostRequestMsg(floor, direction, fromIP);
	udp_send(msg.c_str(), strlen(msg.c_str()) + 1);
}

void msgTool_sendOrderCostReply(Offer offer, std::string fromIP){
	std::string msg;
	msg = msgParser_makeOrderCostReplyMsg(offer, fromIP);
	udp_send(msg.c_str(), strlen(msg.c_str()) + 1);
}

