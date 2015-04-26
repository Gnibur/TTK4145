#pragma once

#include "DataStructures.h"


#define AUCTION_TIME 2

void auction_start(int floor, button_type_t direction);

void auction_addBid(Offer offer);
