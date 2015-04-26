#include <pthread.h>

#include "udp.h"
#include "MsgParser.h"
#include "OrderManager.h"
#include "StateMachine.h"
#include "AuctionManager.h"

#include <iostream>
#include <unistd.h>


#define BUFFERLENGTH 4096

void *listen(void*);

static void synchronizeLists(std::string message);

bool networkListener_run()
{
	if (udp_initialize(BROADCAST_PORT) == false)
		return false;
	
	pthread_t networkListener;
	if (pthread_create(&networkListener, NULL, listen, NULL) != 0)
		return false;

	return true;
}


void *listen(void*) 
{
	char messagebuffer[BUFFERLENGTH];
	
	usleep(1000);

	while (true) {
		udp_receive(messagebuffer, BUFFERLENGTH); // blocking 

		std::string senderIP;
		if (msgParser_getSenderIP(messagebuffer, &senderIP) == false)
			continue;

		if (senderIP == getMyIP())
			continue;

		MsgType messageType;
		if (msgParser_getMessageType(messagebuffer, &messageType) == false)
			continue;

		switch (messageType) {

		case NEW_ORDER_MSG: {
			Order order;
			if (msgParser_getOrderFromMessage(messagebuffer, &order) == false)	
				continue;
		
			orderManager_addOrder(order, DONT_SEND_UPDATE);
				
			synchronizeLists(messagebuffer);
	
			FSM_handleNewOrderArrived(order);

			break;
		}
		case CLEAR_ORDER_MSG: {
			Order order;
			if (msgParser_getOrderFromMessage(messagebuffer, &order) == false)
				continue;

			orderManager_clearOrder(order, DONT_SEND_UPDATE);	
				
			synchronizeLists(messagebuffer);

			break;
		}

		case ORDER_COST_REQUEST: {	
			int floor;
			button_type_t direction; 

			if (msgParser_getOrderCostRequestFromMessage(messagebuffer, &floor, &direction) == false)
				continue;
			
			FSM_handleAuctionStarted(floor, direction);
			
			break;
		}

		case ORDER_COST_REPLY: {
			
			Offer offer;
			if (msgParser_getOfferFromMessage(messagebuffer, &offer) == false)
				continue;
			
			auction_addBid(offer);
			break;
	 	}
		case ORDER_LIST_MSG:
			synchronizeLists(messagebuffer);
			break;

		default:
			// Unknown message
            break;
		}


    
	}
	return NULL;

}

void synchronizeLists(std::string message)
{
	OrderList receivedOrderList;
	if (msgParser_getOrderListFromMessage(message, &receivedOrderList) == false)
		return;

	orderManager_mergeOrderListWith(receivedOrderList, SEND_UPDATE);
}

