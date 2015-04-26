#include "AuctionManager.h"
#include "MsgParser.h"
#include "udp.h"
#include "OrderManager.h"
#include "StateMachine.h"
#include <pthread.h>
#include <time.h>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <iostream>

std::map<Order, std::vector<Offer>> auctions;

static pthread_mutex_t auctionMutex = PTHREAD_MUTEX_INITIALIZER;

void *runAuction(void *args);

void auction_start(int floor, button_type_t direction)
{
	Order *order = new Order(floor, direction, "");
	
	pthread_mutex_lock(&auctionMutex);

	if (auctions.find(*order) == auctions.end()){
		auctions[*order] = std::vector<Offer>();		
		pthread_t auction_id;
		pthread_create(&auction_id, NULL, runAuction, (void*)order);
	}
	else {
		delete order;
	}
	pthread_mutex_unlock(&auctionMutex);
}

void *runAuction(void *args)
{
	Order *order = (Order*)args;

	std::cout << "AUCTION STARTED for floor " << order->floor << ", dir " << order->direction << std::endl;

	std::string costRequestMsg;
	costRequestMsg = msgParser_makeOrderCostRequestMsg(order->floor, order->direction, getMyIP());
	udp_send(costRequestMsg.c_str(), strlen(costRequestMsg.c_str()) + 1);
	
	time_t timeAtStart = time(0);
	while (time(0) < timeAtStart + AUCTION_TIME)
		;
		
	pthread_mutex_lock(&auctionMutex);

	if (auctions[(*order)].size() == 0)
		return NULL;

	std::sort(auctions[(*order)].begin(), auctions[(*order)].end());
	Offer bestOffer = auctions[(*order)][0];
	auctions.erase(*order);

	pthread_mutex_unlock(&auctionMutex);
	
	order->assignedIP = bestOffer.fromIP;
	
	orderManager_addOrder(*order, SEND_UPDATE);
	FSM_handleNewOrderArrived(*order);
	
	
	std::cout 	<< "AUCTION FINISHED, for floor " << order->floor << ", dir " << order->direction 
            	<< ", IP " << bestOffer.fromIP << ", cost "<< bestOffer.cost << " won\n";

	delete order;
	return NULL;
}

void auction_addBid(Offer offer)
{
	Order order(offer.floor, offer.direction,  "");
	
	pthread_mutex_lock(&auctionMutex);
	if (auctions.find(order) != auctions.end())
		auctions[order].push_back(offer);

	std::cout << "Got offer from " << offer.fromIP << ", floor " << offer.floor << ", cost" << offer.cost << std::endl;
	pthread_mutex_unlock(&auctionMutex);
	
}
