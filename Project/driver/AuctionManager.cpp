#include "AuctionManager.h"
#include "DataStructures.h"
#include "udp.h"
#include "OrderManager.h"
#include "MsgParser.h"
#include "StateMachine.h"
#include <pthread.h>
#include <time.h>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstring>
#include <iostream>

std::map<Order, std::vector<Offer>> auctions;

static pthread_mutex_t auctionMutex;

void *runAuction(void *args);

void auctionManager_init()
{
	pthread_mutex_init(&auctionMutex, NULL);
}


void auction_start(int floor, button_type_t direction)
{
	Order *order = new Order(direction, floor, "", -1);
	
	pthread_mutex_lock(&auctionMutex);

	if (auctions.find(*order) == auctions.end()){
		int cost = orderManager_getCost(getLastFloor(), floor, getLastDirection(), direction);
		
		Offer offer(cost, floor, direction, udp_myIP());
		auctions[*order] = std::vector<Offer>();
		auctions[*order].push_back(offer);
		
		pthread_t auction_id;
		pthread_create(&auction_id, NULL, runAuction, (void*)order);
	}
	else {
		std::cout << "Already found order\n";
		delete order;
	}
	pthread_mutex_unlock(&auctionMutex);
}

void *runAuction(void *args)
{
	Order *order = (Order*)args;

	std::cout << "AUCTION STARTED for floor " << order->floor << ", dir " << order->direction << std::endl;

	std::string orderCostRequestMsg;
	orderCostRequestMsg = msgParser_makeOrderCostRequestMsg(order->floor, order->direction, udp_myIP());
	udp_send(orderCostRequestMsg.c_str(), strlen(orderCostRequestMsg.c_str()) + 1);
	
	time_t timeAtStart = time(0);
	while (time(0) < timeAtStart + AUCTION_TIME)
		;
	
	std::cout << "Now finding best offer...\n";
	
	pthread_mutex_lock(&auctionMutex);

	std::sort(auctions[(*order)].begin(), auctions[(*order)].end());
	Offer bestOffer = auctions[(*order)][0];
	auctions.erase(*order);

	pthread_mutex_unlock(&auctionMutex);
	
	order->assignedIP = bestOffer.fromIP;
	order->timeAssigned = time(0);

	
	orderManager_newOrder(*order);
	std::string newOrderMessage;
	newOrderMessage = msgParser_makeNewOrderMsg(*order, orderManager_getOrders(), udp_myIP());
	udp_send(newOrderMessage.c_str(), strlen(newOrderMessage.c_str()) + 1);
	stateMachine_newOrder(*order);

	
	
	std::cout 	<< "AUCTION FINISHED, for floor " << order->floor << ", dir " << order->direction 
            	<< ", IP " << bestOffer.fromIP << " won\n";

	delete order;
	return NULL;
}

void auction_addBid(Offer offer)
{
	Order order(offer.direction, offer.floor, "", -1);

	pthread_mutex_lock(&auctionMutex);
	if (auctions.find(order) != auctions.end())
		auctions[order].push_back(offer);
	std::cout << "THE NUMBER OF BIDS IS NOW " << auctions.size() << std::endl; 
	pthread_mutex_unlock(&auctionMutex);
	
}
