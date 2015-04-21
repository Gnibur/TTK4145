#include "MsgParser.h"
#include "tinyxml.h"


MsgType MsgParser::getMessageType(string message)
{
	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());
	TiXmlElement *elem = xmldoc.RootElement();

	int temp;
	elem->QueryIntAttribute("Type", &temp);
	return (MsgType)temp;
}


Order MsgParser::getOrderFromMessage(string message)
{
	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());


	TiXmlElement *elem = xmldoc.RootElement();

	int temp;
	Order order;
	elem->QueryIntAttribute("Direction", &temp);
	order.direction = (button_type_t)temp;
	elem->QueryIntAttribute("Floor", &order.floor);

	if (getMessageType(message) == CLEAR_ORDER_MSG)
		order.assignedIP = elem->Attribute("AssignedIP");

	return order;
}


OrderList MsgParser::getOrderListFromMessage(string message)
{
	//  printf("Received message: %s\n", message.c_str());

	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());
	TiXmlElement *root = xmldoc.RootElement();

	OrderList orderlist;

	TiXmlElement *orderListRoot = root->NextSibling("OrderList")->ToElement();
	TiXmlNode *childNode = NULL;
	while (childNode = orderListRoot->IterateChildren(childNode)){
		TiXmlElement *childElement = childNode->ToElement();
		int temp;
		Order order;
		childElement->QueryIntAttribute("Direction", &temp);
		order.direction = (button_type_t)temp;
		childElement->QueryIntAttribute("Floor", &order.floor);

		if (getMessageType(message) != NEW_ORDER_MSG)
			order.assignedIP = childElement->Attribute("AssignedIP");
		orderlist.push_back(order);
	}
	return orderlist;
}


Order MsgParser::getOrderCostRequestFromMessage(string message)
{
	Order order;

	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());

	TiXmlElement *elem = xmldoc.RootElement();

	int temp;
	elem->QueryIntAttribute("Floor", &temp);
	order.floor = temp;
	elem->QueryIntAttribute("Direction", &temp);
	order.direction = (button_type_t)temp;


	return order;
}

Offer MsgParser::getOrderCostReplyFromMessage(string message)
{
	Offer offer;

	TiXmlDocument xmldoc;
	xmldoc.Parse(message.c_str());

	TiXmlElement *elem = xmldoc.RootElement();

	int temp;
	elem->QueryIntAttribute("Cost", &offer.cost);
	elem->QueryIntAttribute("Floor", &offer.floor);
	elem->QueryIntAttribute("Direction", &temp);
	offer.direction = (button_type_t)temp;

	offer.fromIP = elem->Attribute("fromIP");

	return offer;
}

/* ------------------Make messages  ---------------------------------- */

string MsgParser::makeNewOrderMsg(Order order, OrderList updatedOrderList)
{
	TiXmlDocument xmldoc;
	TiXmlElement *root = new TiXmlElement("Order");

	root->SetAttribute("Type", NEW_ORDER_MSG);
	root->SetAttribute("Direction", order.direction);
	root->SetAttribute("Floor", order.floor);
	xmldoc.LinkEndChild(root);


	TiXmlElement *orderList = new TiXmlElement("OrderList");
	xmldoc.LinkEndChild(orderList);
	for (auto it = updatedOrderList.begin(); it != updatedOrderList.end(); ++it){
		TiXmlElement *orderListElement = new TiXmlElement("Order");
		orderListElement->LinkEndChild(orderListElement);
		orderListElement->SetAttribute("Direction", it->direction);
		orderListElement->SetAttribute("Floor", it->floor);
		orderListElement->SetAttribute("AssignedIP", it->assignedIP.c_str());
	}

	TiXmlPrinter printer;
	xmldoc.Accept(&printer);
	return printer.CStr();
}

string MsgParser::makeClearOrderMsg(Order order, OrderList updatedOrderList)
{
	TiXmlDocument xmldoc;
	TiXmlElement *root = new TiXmlElement("Order");

	xmldoc.LinkEndChild(root);
	root->SetAttribute("Type", CLEAR_ORDER_MSG);
	root->SetAttribute("Direction", order.direction);
	root->SetAttribute("Floor", order.floor);

	TiXmlElement *orderList = new TiXmlElement("OrderList");
	xmldoc.LinkEndChild(orderList);
	for (auto it = updatedOrderList.begin(); it != updatedOrderList.end(); ++it){
		TiXmlElement *orderListElement = new TiXmlElement("Order");
		orderListElement->LinkEndChild(orderListElement);
		orderListElement->SetAttribute("Direction", it->direction);
		orderListElement->SetAttribute("Floor", it->floor);
		orderListElement->SetAttribute("AssignedIP", it->assignedIP.c_str());
	}

	TiXmlPrinter printer;
	xmldoc.Accept(&printer);
	return printer.CStr();
}

string MsgParser::makeOrderListMsg(OrderList orders)
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
		orderListElement->SetAttribute("Direction", it->direction);
		orderListElement->SetAttribute("Floor", it->floor);
		orderListElement->SetAttribute("AssignedIP", it->assignedIP.c_str());
	}

	TiXmlPrinter printer;
	xmldoc.Accept(&printer);
	return printer.CStr();
}

string MsgParser::makeOrderCostRequestMsg(int floor, button_type_t direction)
{
	TiXmlDocument xmldoc;
	TiXmlElement *msg = new TiXmlElement("RootElement");
	xmldoc.LinkEndChild(msg);
	msg->SetAttribute("Type", ORDER_COST_REQUEST);
	msg->SetAttribute("Floor", floor);
	msg->SetAttribute("Direction", direction);

	TiXmlPrinter printer;
	xmldoc.Accept(&printer);

	return printer.CStr();
}

string MsgParser::makeOrderCostReplyMsg(int cost, int floor, button_type_t dir, string IP)
{
	TiXmlDocument xmldoc;
	TiXmlElement *msg = new TiXmlElement("RootElement");

	xmldoc.LinkEndChild(msg);

	msg->SetAttribute("Type", ORDER_COST_REPLY);
	msg->SetAttribute("Cost", cost);
	msg->SetAttribute("Floor", floor);
	msg->SetAttribute("Direction", dir);
	msg->SetAttribute("fromIP", IP.c_str());

	TiXmlPrinter printer;
	xmldoc.Accept(&printer);

	return printer.CStr();
}
