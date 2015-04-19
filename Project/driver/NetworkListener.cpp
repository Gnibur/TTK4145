
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
			order = getOrderFromMessage(buf);
			orderManager.newOrder(order);
			break;

		case GET_FLOOR_COST:
			cost = orderManager.getCost(currentFloor, currentDirection); // global variables
			sendOffer(cost);
			break;

		case REMOVE_ORDER:
			order = getOrderFromMessage(buf);
			orderManager.removeOrder(order);
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
		if (receivedOrderList != orderManager.getOrders())
		{
			orderManager.mergeMyOrdersWith(receivedOrderList);
			sendUpdate(orderManager.getOrders());
		}

	}

}

