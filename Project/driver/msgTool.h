#pragma once 

#include "Enums.h"
#include "DataStructures.h"
#include <string>

void msgTool_sendNewOrder(Order order, OrderList updatedOrderList, std::string fromIP);

void msgTool_sendClearOrder(Order order, OrderList updatedOrderList, std::string fromIP);

void msgTool_sendOrderList(OrderList orderList, std::string fromIP);

void msgTool_sendOrderCostRequest(int floor, button_type_t direction, std::string fromIP);

void msgTool_sendOrderCostReply(Offer offer, std::string IP);



