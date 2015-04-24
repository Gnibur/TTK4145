#include "MsgParser.h"
#include "tinyxml.h"
#include "udp.h"
#include "IoDriver.h" // remove when N_FLOORS is moved
#include <iostream>


bool msgParser_getMessageType(string message, MsgType *msgTypeFromMessage)
{
	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());

	TiXmlElement *xmlHeader = xmldoc.RootElement();

	if (xmldoc.Error() ||  (!xmlHeader))
		return false;

	int temp;
	xmlHeader->QueryIntAttribute("Type", &temp);
	*msgTypeFromMessage = (MsgType)temp;

	return true;
}

bool msgParser_getSenderIP(string message, std::string *senderIP)
{
	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());

	TiXmlElement *xmlHeader = xmldoc.RootElement();
	
	if (xmldoc.Error() ||  (!xmlHeader))
		return false;

	*senderIP = xmlHeader->Attribute("fromIP");
	
	if (isValidIP(*senderIP))
		return true;
	else
		return false;
}

bool msgParser_getOrderFromMessage(string message, Order *order)
{
	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());

	TiXmlElement *xmlHeader = xmldoc.RootElement();
	
	if (xmldoc.Error() ||  (!xmlHeader))
		return false;

	TiXmlNode *xmlOrder = xmlHeader->NextSibling("Order");
	
	if (xmlOrder){
		xmlOrder->ToElement()->QueryIntAttribute("Floor", &order->floor);
		int temp;
		xmlOrder->ToElement()->QueryIntAttribute("Direction", &temp);
		order->direction = (button_type_t)temp;
		order->assignedIP = xmlOrder->ToElement()->Attribute("AssignedIP");
	} 
	
	if (order->isValid())
		return true;
	else
		return false;	
}


bool msgParser_getOrderListFromMessage(string message, OrderList *orderlist)
{
	//  printf("Received message: %s\n", message.c_str());
	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());
	TiXmlElement *xmlHeader = xmldoc.RootElement();
	
	if (xmldoc.Error() ||  (!xmlHeader))
		return false;

	TiXmlNode *orderListRoot = xmlHeader->NextSibling("OrderList");

	if (!orderListRoot)
		return false;
	
	TiXmlNode *childNode = NULL;
	while (childNode = ((orderListRoot->ToElement())->IterateChildren(childNode))){
		
		TiXmlElement *childElement = childNode->ToElement();

		int temp;
		Order order;
		childElement->QueryIntAttribute("Floor", &order.floor);
		childElement->QueryIntAttribute("Direction", &temp);
		order.direction = (button_type_t)temp;
		order.assignedIP = childElement->Attribute("AssignedIP");

		if (order.isValid() == false)
			return false;
		else 
			orderlist->push_back(order);
	}
	return true;
}


bool msgParser_getOrderCostRequestFromMessage(string message, int *floor, button_type_t *direction)
{
	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());

	TiXmlElement *xmlHeader = xmldoc.RootElement();

	if (xmldoc.Error() ||  (!xmlHeader))
		return false;

	TiXmlNode *xmlOrderCostRequest = xmlHeader->NextSibling("OrderCostRequest");

	if (!xmlOrderCostRequest)
		return false;
	
	int temp;
	xmlOrderCostRequest->ToElement()->QueryIntAttribute("Floor", floor);
	xmlOrderCostRequest->ToElement()->QueryIntAttribute("Direction", &temp);
	*direction = (button_type_t)temp;

	if (*floor >= 0 && *floor < N_FLOORS)
		return true;
	else 
		return false;
}

bool msgParser_getOfferFromMessage(string message, Offer *offer)
{
	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());

	TiXmlElement *xmlHeader = xmldoc.RootElement();

	if (xmldoc.Error() ||  (!xmlHeader))
		return false;

	TiXmlNode *xmlOrderCostOffer = xmlHeader->NextSibling("OrderCostReply");

	int temp;
	xmlOrderCostOffer->ToElement()->QueryIntAttribute("Cost", &offer->cost);
	xmlOrderCostOffer->ToElement()->QueryIntAttribute("Floor", &offer->floor);
	xmlOrderCostOffer->ToElement()->QueryIntAttribute("Direction", &temp);
	offer->direction = (button_type_t)temp;

	offer->fromIP = xmlOrderCostOffer->ToElement()->Attribute("fromIP");

	if (offer->isValid())
		return true;
	else 
		return false;
}

/* ------------------Make messages  ---------------------------------- */

static TiXmlElement * msgParser_buildMessageHeader(MsgType type, std::string fromIP)
{
	TiXmlElement *rootElement = new TiXmlElement("MessageHeader\n");
	
	rootElement->SetAttribute("Type", type);
	rootElement->SetAttribute("fromIP", fromIP.c_str());

	return rootElement;			 
}

static TiXmlElement * msgParser_buildOrderElement(Order order) 
{
	TiXmlElement *xmlOrder = new TiXmlElement("Order");
	xmlOrder->SetAttribute("Floor", order.floor);
	xmlOrder->SetAttribute("Direction", order.direction);
	xmlOrder->SetAttribute("AssignedIP", order.assignedIP.c_str());
	
	return xmlOrder;
}

static TiXmlElement * msgParser_buildOrderListElement(OrderList orderList)
{
	TiXmlElement *xmlOrderList = new TiXmlElement("OrderList");

	for (auto it = orderList.begin(); it != orderList.end(); ++it){
		TiXmlElement *orderListElement = msgParser_buildOrderElement(*it);
		xmlOrderList->LinkEndChild(orderListElement);
	}

	return xmlOrderList;
}


string msgParser_makeNewOrderMsg(Order order, OrderList updatedOrderList)
{
	TiXmlDocument xmldoc;

	TiXmlElement *xmlHeader = msgParser_buildMessageHeader(NEW_ORDER_MSG, getMyIP());
	TiXmlElement *xmlOrder = msgParser_buildOrderElement(order);
	TiXmlElement *xmlOrderList = msgParser_buildOrderListElement(updatedOrderList);
	
	xmldoc.LinkEndChild(xmlHeader);
	xmldoc.LinkEndChild(xmlOrder);
	xmldoc.LinkEndChild(xmlOrderList);

	TiXmlPrinter printer;
	xmldoc.Accept(&printer);
	return printer.CStr();
}

string msgParser_makeClearOrderMsg(Order order, OrderList updatedOrderList)
{
	TiXmlDocument xmldoc;

	TiXmlElement *xmlHeader = msgParser_buildMessageHeader(CLEAR_ORDER_MSG, getMyIP());
	TiXmlElement *xmlOrder = msgParser_buildOrderElement(order);
	TiXmlElement *xmlOrderList = msgParser_buildOrderListElement(updatedOrderList);
	
	xmldoc.LinkEndChild(xmlHeader);
	xmldoc.LinkEndChild(xmlOrder);
	xmldoc.LinkEndChild(xmlOrderList);

	TiXmlPrinter printer;
	xmldoc.Accept(&printer);
	return printer.CStr();
}

string msgParser_makeOrderListMsg(OrderList orders)
{
	TiXmlDocument xmldoc;

	TiXmlElement *xmlHeader = msgParser_buildMessageHeader(CLEAR_ORDER_MSG, getMyIP());
	TiXmlElement *xmlOrderList = msgParser_buildOrderListElement(orders);

	xmldoc.LinkEndChild(xmlHeader);
	xmldoc.LinkEndChild(xmlOrderList);

	TiXmlPrinter printer;
	xmldoc.Accept(&printer);
	return printer.CStr();
}

string msgParser_makeOrderCostRequestMsg(int floor, button_type_t direction)
{
	TiXmlDocument xmldoc;

	TiXmlElement *xmlHeader = msgParser_buildMessageHeader(ORDER_COST_REQUEST, getMyIP());

	TiXmlElement *xmlCostRequest = new TiXmlElement("OrderCostRequest");
	xmlCostRequest->SetAttribute("Floor", floor);
	xmlCostRequest->SetAttribute("Type", ORDER_COST_REQUEST);
	xmlCostRequest->SetAttribute("Direction", direction);

	xmldoc.LinkEndChild(xmlHeader);
	xmldoc.LinkEndChild(xmlCostRequest);
	
	TiXmlPrinter printer;
	xmldoc.Accept(&printer);
	return printer.CStr();
}

string msgParser_makeOrderCostReplyMsg(Offer offer)
{
	TiXmlDocument xmldoc;

	TiXmlElement *xmlHeader = msgParser_buildMessageHeader(ORDER_COST_REPLY, getMyIP());

	TiXmlElement *xmlCostReply = new TiXmlElement("OrderCostReply");
	xmlCostReply->SetAttribute("Type", ORDER_COST_REPLY);
	xmlCostReply->SetAttribute("Cost", offer.cost);
	xmlCostReply->SetAttribute("Floor", offer.floor);
	xmlCostReply->SetAttribute("Direction", offer.direction);
	xmlCostReply->SetAttribute("fromIP", offer.fromIP.c_str());

	xmldoc.LinkEndChild(xmlHeader);
	xmldoc.LinkEndChild(xmlCostReply);

	TiXmlPrinter printer;
	xmldoc.Accept(&printer);

	return printer.CStr();
}
