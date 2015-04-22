#include <pthread.h>

#include "udp.h"
#include "MsgParser.h"
#include "OrderManager.h"
#include "StateMachine.h"
#include "AuctionManager.h"

#include <cstring>
#include <iostream>

#define BUFLENGTH 1024

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
		udp_receive(BROADCAST_PORT, buf, BUFLENGTH); // blocking read into buf
		MsgType messageType = msgParser_getMessageType(buf);
        
		switch (messageType) {

		case NEW_ORDER_MSG:{
			Order order = msgParser_getOrderFromMessage(buf);
			orderManager_newOrder(order);
			stateMachine_newOrder(order.floor, order.direction);
			break;
		}
		case ORDER_COST_REQUEST: {
			Order order				= msgParser_getOrderCostRequestFromMessage(buf);
			int cost					= orderManager_getCost(getLastFloor(), order.floor, getLastDirection(), order.direction);
			Offer offer				= {cost, order.floor, order.direction, getMyIP()};
			std::string offerMsg = msgParser_makeOrderCostReplyMsg(offer);
			udp_send(BROADCAST_PORT, offerMsg.c_str(), strlen(offerMsg.c_str()));
			break;
		}
		case CLEAR_ORDER_MSG: {
			Order order = msgParser_getOrderFromMessage(buf);
			orderManager_clearOrder(order);
			break;
		}
		case ORDER_COST_REPLY: {
			Offer offer = msgParser_getOfferFromMessage(buf);
			auction_addBid(offer);
			break;
	 	}
		case ORDER_LIST_MSG:
			// we want to merge anyway. That happens below

		default:
            //std::cout << "Unknown message received!\n";
			// Unknown message
            continue;
		}

		OrderList receivedOrderList = msgParser_getOrderListFromMessage(buf);
		if (!(orderManager_orderListEquals(receivedOrderList)))
		{
			orderManager_mergeMyOrdersWith(receivedOrderList);

			std::string updateMsg = msgParser_makeOrderListMsg(orderManager_getOrders());
			udp_send(BROADCAST_PORT, updateMsg.c_str(), strlen(updateMsg.c_str()));	
		}
    
	}
	return NULL;

}

