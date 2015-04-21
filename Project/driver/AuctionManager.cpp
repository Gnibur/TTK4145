#include "AuctionManager.h"
#include "DataStructures.h"
#include "MsgParser.h"
#include <pthread.h>
#include <time.h>
#include <map>
#include <vector>
#include <string>

std::map<Order, std::vector<Offer> auctions;

static pthread_mutex_t auctionMutex;

void *runAuction(void *args);

void auctionManager_init()
{
  pthread_mutex_init(&auctionMutex, NULL);
}


void auction_start(int floor, button_type_t direction)
{
  Order order = {direction, floor};
  pthread_t auction_id;
  pthread_create(&auction_id, NULL, runAuction, (void*)order);
}

void *runAuction(void *args)
{
  Order order = (order)args;

  time_t timeAtStart = time(0);

  while (time(0) < timeAtStart + 3)
    ;

  pthread_mutex_lock(&auctionMutex);

  std::sort(auctions[order].begin(), auctions[order].end());
  
  // hopefully there are some bids
  Offer bestOffer = auctions[order][0];
  
  pthread_mutex_unlock(&auctionMutex);
  
  order.assignedIP = bestOffer.fromIP;
  MsgParser parser;
  std::string newOrderMessage = parser.makeNewOrderMsg(order, updatedList);
  udp_send(BROADCAST_PORT; newOrderMessage.c_str(), strlen(newOrderMessage.c_str());
}

void auction_addBid(Offer offer)
{
  Order order = {offer.direction, offer.floor};

  pthread_mutex_lock(&auctionMutex);
  if (auctions.find(order) != auctions.end())
    auctions[order].push_back(offer);
  pthread_mutex_unlock(&auctionMutex);
}
