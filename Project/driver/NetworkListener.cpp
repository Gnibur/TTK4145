#include <pthread.h>

#include "udp.h"
#include "MsgParser.h"
#include "OrderManager.h"
#include "StateMachine.h"
#include "AuctionManager.h"

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <ctime>

#define BUFLENGTH 4096

void *listen(void*);

void networkListener_run()
{
	pthread_t networkListener;
	pthread_create(&networkListener, NULL, listen, NULL);
}


void *listen(void*) 
{
	char buf[BUFLENGTH];
	while (true) {
		udp_receive(buf, BUFLENGTH); // blocking read into buf

		std::string senderIP;
		if (msgParser_getSenderIP(buf, &senderIP) == false)
			continue;

		if (senderIP == udp_myIP())
			continue;

		MsgType messageType;
		if (msgParser_getMessageType(buf, &messageType) == false)
			continue;

		switch (messageType) {

		case NEW_ORDER_MSG:{
			std::cout << "Received New order:\n" << buf << "\n\n";
			
			Order order;
			if (msgParser_getOrderFromMessage(buf, &order) == false){
				std::cout << "Failed to obtain order from message\n";				
				continue;
			}
			int timeNow = time(0);
			order.timeAssigned = timeNow;
			orderManager_newOrder(order);
			stateMachine_newOrder(order);
			std::cout << "Order was added\n";
			break;
		}
		case CLEAR_ORDER_MSG: {
			std::cout << "Received Clear order:\n" << buf << "\n\n";

			Order order;
			if (msgParser_getOrderFromMessage(buf, &order) == false)
				continue;

			orderManager_clearOrder(order);
			std::cout << "Order was cleared\n";
			break;
		}

		case ORDER_COST_REQUEST: {
			std::cout << "Received Order cost request:\n" << buf << "\n\n";

			int floor;
			button_type_t direction; 


			if (msgParser_getOrderCostRequestFromMessage(buf, &floor, &direction) == false)
				continue;

			int cost = orderManager_getCost(getLastFloor(), floor, getLastDirection(), direction);
			
			Offer offer(cost, floor, direction, udp_myIP());
			std::string offerMsg = msgParser_makeOrderCostReplyMsg(offer, udp_myIP());

			udp_send(offerMsg.c_str(), strlen(offerMsg.c_str()) + 1);
			//usleep(10000);
			break;
		}

		case ORDER_COST_REPLY: {
			std::cout << "Received Order cost reply:\n" << buf << "\n\n";

			Offer offer;
			if (msgParser_getOfferFromMessage(buf, &offer))
				continue;

			auction_addBid(offer);
			break;
	 	}
		case ORDER_LIST_MSG:
			std::cout << "Received list:\n" << buf << std::endl;
			// we want to merge anyway. That happens below
			break;

		default:
            std::cout << "Unknown message received:\n" << buf << std::endl;;
			// Unknown message
            continue;
		}

		if ((messageType != ORDER_COST_REQUEST) && (messageType != ORDER_COST_REPLY))
		{
			OrderList receivedOrderList;
			if (msgParser_getOrderListFromMessage(buf, &receivedOrderList) == false)
				continue;
			//OrderList receivedOrderList = orderManager_getOrders();
			if (!(orderManager_orderListEquals(receivedOrderList)))
			{
				orderManager_mergeMyOrdersWith(receivedOrderList);
				//usleep(10000);
			}
		}
    
	}
	return NULL;

}

