#include <pthread.h>

#include "udp.h"

#include "msgTool.h"
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

static void synchronizeLists(std::string message);

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

		case NEW_ORDER_MSG: {
			std::cout << "Received New order:\n" << buf << "\n\n";
			
			Order order;
			if (msgParser_getOrderFromMessage(buf, &order) == false){
				std::cout << "Failed to obtain order from message\n";				
				continue;
			}
			if (orderManager_addOrder(order, DONT_SEND_UPDATE))
				std::cout << "Order was added\n";

			synchronizeLists(buf);
	
			stateMachine_eventNewOrderArrived(order);

			break;
		}
		case CLEAR_ORDER_MSG: {
			std::cout << "Received Clear order:\n" << buf << "\n\n";

			Order order;
			if (msgParser_getOrderFromMessage(buf, &order) == false)
				continue;

			if (orderManager_clearOrder(order, DONT_SEND_UPDATE))	
				std::cout << "Order was cleared\n";

			synchronizeLists(buf);
			
			break;
		}

		case ORDER_COST_REQUEST: {
			std::cout << "Received Order cost request:\n" << buf << "\n\n";

			int floor;
			button_type_t direction; 


			if (msgParser_getOrderCostRequestFromMessage(buf, &floor, &direction) == false)
				continue;

			stateMachine_eventAuctionStarted(floor, direction);
			
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
			
			synchronizeLists(buf);
			break;

		default:
            std::cout << "Unknown message received:\n" << buf << std::endl;;
			// Unknown message
            break;
		}


    
	}
	return NULL;

}

void synchronizeLists(std::string message)
{
	/*OrderList receivedOrderList;
	if (msgParser_getOrderListFromMessage(message, &receivedOrderList) == false)
		return;


	//OrderList receivedOrderList = orderManager_getOrders();
	if (!(orderManager_orderListEquals(receivedOrderList)))
	{
		std::cout << "OrderList is not equal!\n";
		orderManager_mergeMyOrdersWith(receivedOrderList, DONT_SEND_UPDATE);
		msgTool_sendOrderList(orderManager_getOrders(), udp_myIP());
	}
	*/


}

