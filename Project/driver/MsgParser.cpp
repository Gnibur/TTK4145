#include "MsgParser.h"
#include "tinyxml.h"
#include <iostream>


MsgType msgParser_getMessageType(string message)
{
	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());
	TiXmlElement *elem = xmldoc.RootElement();
	if (!elem)
		return INVALID_MESSAGE;
	int temp;
	elem->QueryIntAttribute("Type", &temp);
	return (MsgType)temp;
}


Order msgParser_getOrderFromMessage(string message)
{
	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());


	TiXmlElement *elem = xmldoc.RootElement();
	if (!elem){
		std::cout << "Could not obtain order from message, root was not found\n";
		return Order();
	}

	int temp;
	Order order;
	elem->QueryIntAttribute("Floor", &order.floor);
	elem->QueryIntAttribute("Direction", &temp);
	order.direction = (button_type_t)temp;
	order.assignedIP = elem->Attribute("AssignedIP");


	/*if (msgParser_getMessageType(message) == CLEAR_ORDER_MSG)
		order.assignedIP = elem->Attribute("AssignedIP");
	*/
	return order;
}


OrderList msgParser_getOrderListFromMessage(string message)
{
	//  printf("Received message: %s\n", message.c_str());
	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());
	TiXmlElement *root = xmldoc.RootElement();
	if (!root)
		return OrderList { }; // ooops, how to return invalid orderlist?

	OrderList orderlist;

	TiXmlElement *orderListRoot = root->NextSibling("OrderList")->ToElement();
	TiXmlNode *childNode = NULL;
	while (childNode = orderListRoot->IterateChildren(childNode)){
		TiXmlElement *childElement = childNode->ToElement();
		int temp;
		Order order;
		childElement->QueryIntAttribute("Floor", &order.floor);
		childElement->QueryIntAttribute("Direction", &temp);
		order.direction = (button_type_t)temp;


		//if (msgParser_getMessageType(message) != NEW_ORDER_MSG)
		order.assignedIP = childElement->Attribute("AssignedIP");
		orderlist.push_back(order);
	}
	return orderlist;
}


Order msgParser_getOrderCostRequestFromMessage(string message)
{
	Order order;

	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());

	TiXmlElement *elem = xmldoc.RootElement();
	if (!elem)
		return Order();

	int temp;
	elem->QueryIntAttribute("Floor", &temp);
	order.floor = temp;
	elem->QueryIntAttribute("Direction", &temp);
	order.direction = (button_type_t)temp;


	return order;
}

Offer msgParser_getOfferFromMessage(string message)
{
	Offer offer;

	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());

	TiXmlElement *elem = xmldoc.RootElement();
	if (!elem)
		return Offer();

	int temp;
	elem->QueryIntAttribute("Cost", &offer.cost);
	elem->QueryIntAttribute("Floor", &offer.floor);
	elem->QueryIntAttribute("Direction", &temp);
	offer.direction = (button_type_t)temp;

	offer.fromIP = elem->Attribute("fromIP");

	return offer;
}

/* ------------------Make messages  ---------------------------------- */

string msgParser_makeNewOrderMsg(Order order, OrderList updatedOrderList)
{
	TiXmlDocument xmldoc;
	TiXmlElement *root = new TiXmlElement("Order");

	root->SetAttribute("Type", NEW_ORDER_MSG);
	root->SetAttribute("Floor", order.floor);
	root->SetAttribute("Direction", order.direction);
	root->SetAttribute("AssignedIP", order.assignedIP.c_str());
	xmldoc.LinkEndChild(root);


	TiXmlElement *orderList = new TiXmlElement("OrderList");
	xmldoc.LinkEndChild(orderList);
	for (auto it = updatedOrderList.begin(); it != updatedOrderList.end(); ++it){
		TiXmlElement *orderListElement = new TiXmlElement("Order");
		orderList->LinkEndChild(orderListElement);
		orderListElement->SetAttribute("Floor", it->floor);
		orderListElement->SetAttribute("Direction", it->direction);
		orderListElement->SetAttribute("AssignedIP", it->assignedIP.c_str());
	}

	TiXmlPrinter printer;
	xmldoc.Accept(&printer);
	return printer.CStr();
}

string msgParser_makeClearOrderMsg(Order order, OrderList updatedOrderList)
{
	TiXmlDocument xmldoc;
	TiXmlElement *root = new TiXmlElement("Order");

	xmldoc.LinkEndChild(root);
	root->SetAttribute("Type", CLEAR_ORDER_MSG);
	root->SetAttribute("Floor", order.floor);
	root->SetAttribute("Direction", order.direction);
	root->SetAttribute("AssignedIP", order.assignedIP.c_str());

	TiXmlElement *orderList = new TiXmlElement("OrderList");
	xmldoc.LinkEndChild(orderList);
	for (auto it = updatedOrderList.begin(); it != updatedOrderList.end(); ++it){
		TiXmlElement *orderListElement = new TiXmlElement("Order");
		orderListElement->SetAttribute("Floor", it->floor);
		orderListElement->SetAttribute("Direction", it->direction);
		orderListElement->SetAttribute("AssignedIP", it->assignedIP.c_str());
		orderList->LinkEndChild(orderListElement);
	}

	TiXmlPrinter printer;
	xmldoc.Accept(&printer);
	return printer.CStr();
}

string msgParser_makeOrderListMsg(OrderList orders)
{
	TiXmlDocument xmldoc;
	TiXmlElement *root = new TiXmlElement("root");
	xmldoc.LinkEndChild(root);
	root->SetAttribute("Type", ORDER_LIST_MSG);

	TiXmlElement *orderList = new TiXmlElement("OrderList");
	xmldoc.LinkEndChild(orderList);
	for (auto it = orders.begin(); it != orders.end(); ++it){
		TiXmlElement *orderListElement = new TiXmlElement("Order");
		orderList->LinkEndChild(orderListElement);
		orderListElement->SetAttribute("Floor", it->floor);
		orderListElement->SetAttribute("Direction", it->direction);
		orderListElement->SetAttribute("AssignedIP", it->assignedIP.c_str());
	}

	TiXmlPrinter printer;
	xmldoc.Accept(&printer);
	return printer.CStr();
}

string msgParser_makeOrderCostRequestMsg(int floor, button_type_t direction)
{
	TiXmlDocument xmldoc;
	TiXmlElement *msg = new TiXmlElement("RootElement");
	xmldoc.LinkEndChild(msg);
	msg->SetAttribute("Floor", floor);
	msg->SetAttribute("Type", ORDER_COST_REQUEST);
	msg->SetAttribute("Direction", direction);

	TiXmlPrinter printer;
	xmldoc.Accept(&printer);

	return printer.CStr();
}

string msgParser_makeOrderCostReplyMsg(Offer offer)
{
	TiXmlDocument xmldoc;
	TiXmlElement *msg = new TiXmlElement("RootElement");

	xmldoc.LinkEndChild(msg);

	msg->SetAttribute("Type", ORDER_COST_REPLY);
	msg->SetAttribute("Cost", offer.cost);
	msg->SetAttribute("Floor", offer.floor);
	msg->SetAttribute("Direction", offer.direction);
	msg->SetAttribute("fromIP", offer.fromIP.c_str());

	TiXmlPrinter printer;
	xmldoc.Accept(&printer);

	return printer.CStr();
}
