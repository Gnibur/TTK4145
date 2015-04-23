#include "MsgParser.h"
#include "tinyxml.h"
#include "udp.h"
#include <iostream>


MsgType msgParser_getMessageType(string message)
{
	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());

	TiXmlElement *xmlHeader = xmldoc.RootElement();

	if (xmldoc.Error() ||  (!xmlHeader))
		return INVALID_MESSAGE;

	int temp;
	xmlHeader->QueryIntAttribute("Type", &temp);
	return (MsgType)temp;
}

std::string msgParser_getSenderIP(string message)
{
	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());

	TiXmlElement *xmlHeader = xmldoc.RootElement();
	
	if (xmldoc.Error() ||  (!xmlHeader))
		return "invalid ip";

	return xmlHeader->Attribute("fromIP");
}

Order msgParser_getOrderFromMessage(string message)
{
	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());

	TiXmlElement *xmlHeader = xmldoc.RootElement();
	
	if (xmldoc.Error() ||  (!xmlHeader)){
		std::cout << "Could not obtain order from message, root was not found\n";
		return Order();
	}

	TiXmlNode *xmlOrder = xmlHeader->NextSibling("Order");
	
	if (xmlOrder){
		int temp;
		Order order;
		xmlOrder->ToElement()->QueryIntAttribute("Floor", &order.floor);
		xmlOrder->ToElement()->QueryIntAttribute("Direction", &temp);
		order.direction = (button_type_t)temp;
		order.assignedIP = xmlOrder->ToElement()->Attribute("AssignedIP");

		return order;
	} else
 		return Order();
	
}


OrderList msgParser_getOrderListFromMessage(string message)
{
	//  printf("Received message: %s\n", message.c_str());
	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());
	TiXmlElement *xmlHeader = xmldoc.RootElement();
	
	if (xmldoc.Error() ||  (!xmlHeader)){
		std::cout << "Could not obtain order from message, root was not found\n";
		return OrderList { }; // ooops, how to return invalid orderlist?
	}

	OrderList orderlist;

	TiXmlNode *orderListRoot = xmlHeader->NextSibling("OrderList");

	if (!orderListRoot){
		std::cout << " Did not find orderlist when parsing message\n";
		return OrderList { };
	}
	
	TiXmlNode *childNode = NULL;
	while (childNode = ((orderListRoot->ToElement())->IterateChildren(childNode))){
		
		TiXmlElement *childElement = childNode->ToElement();

		int temp;
		Order order;
		childElement->QueryIntAttribute("Floor", &order.floor);
		childElement->QueryIntAttribute("Direction", &temp);
		order.direction = (button_type_t)temp;
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

	TiXmlElement *xmlHeader = xmldoc.RootElement();

	if (xmldoc.Error() ||  (!xmlHeader)){
		std::cout << "Could not obtain order from message, root was not found\n";
		return Order(); // ooops, how to return invalid orde
	}

	TiXmlNode *xmlOrderCostRequest = xmlHeader->NextSibling("OrderCostRequest");

	if (!xmlOrderCostRequest){
		std::cout << " Did not find order when parsing OrderCostRequest\n";
		return Order { };
	}
	
	int temp;
	xmlOrderCostRequest->ToElement()->QueryIntAttribute("Floor", &temp);
	order.floor = temp;
	xmlOrderCostRequest->ToElement()->QueryIntAttribute("Direction", &temp);
	order.direction = (button_type_t)temp;

	return order;
}

Offer msgParser_getOfferFromMessage(string message)
{
	Offer offer;

	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());

	TiXmlElement *xmlHeader = xmldoc.RootElement();

	if (xmldoc.Error() ||  (!xmlHeader)){
		std::cout << "Could not obtain offer from message, root was not found\n";
		return offer; // ooops, how to return invalid offer?
	}

	TiXmlNode *xmlOrderCostOffer = xmlHeader->NextSibling("OrderCostReply");

	int temp;
	xmlOrderCostOffer->ToElement()->QueryIntAttribute("Cost", &offer.cost);
	xmlOrderCostOffer->ToElement()->QueryIntAttribute("Floor", &offer.floor);
	xmlOrderCostOffer->ToElement()->QueryIntAttribute("Direction", &temp);
	offer.direction = (button_type_t)temp;

	offer.fromIP = xmlOrderCostOffer->ToElement()->Attribute("fromIP");

	return offer;
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
