#include "StateMachine.h"

StateMachine::StateMachine();			// Init function

void StateMachine::newOrder(int floor)
{
	
}
// Send update to others and find new order
void StateMachine::floorReached();			// Send update to others and find new order
void StateMachine::orderButtonPressed();	// Send msg about changing the light
