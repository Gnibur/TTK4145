#pragma once

#include "DataStructures.h"

void auctionManager_init();

void auction_start(int floor, button_type_t direction);

void auction_addBid(Offer offer);