
void run()
{
	/* set up network */
}


void listen() 
{
	while (true) {
		socket.read(buf); // blocking read into buf

		messageType = getMessageType(buf);
  
		switch (messageType) {

		case ADD_ORDER:
			Order order = getOrderFromMessage(buf);
			orderManager.newOrder(order);
			break;

		case GET_FLOOR_COST:
			Order order				= getOrderCostRequestFromMessage(buf);
			cost					= orderManager.getCost(currentFloor, order.floor, currentDirection, order.direction);
			Offer offer				= {cost, order.floor, order.direction, IP};
			std::string offerMsg; // TODO: Fill this out
			udp.send(BROADCAST_PORT, offerMsg, MAXLENGTH_BUF)
			break;

		case REMOVE_ORDER:
			order = getOrderFromMessage(buf);
			orderManager.clearOrder(order);
			break;

		case ORDER_COST_OFFER: 
			offer = getOfferFromMessage(buf);
			budmanager.addOffer(offer);
			break;

		case UPDATE_LIST:
			// we want to merge anyway. That happens below

		default:
			// Unknown message
			return;
		}

		receivedOrderList = getOrderListFromMessage(buf);
		if (!(orderManager.orderListEquals(receivedOrderList)))
		{
			orderManager.mergeMyOrdersWith(receivedOrderList);
			sendUpdate(orderManager.getOrders());
		}

	}

}

