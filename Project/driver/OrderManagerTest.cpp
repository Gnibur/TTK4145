#include <iostream>
#include "OrderManager.h"

using namespace std;

int main () {
	OrderManager my_manager;
	int cmd = 0;
	while (cmd != -1)
	{
		cout << "Please choose what to do: \n";
		cout << "1: Add order \n";
		cout << "2: Run algorithm\n";
		cout << "3: Clear orders \n";
		cout << "4: Print orders\n";
		cout << "-1: Exit\n";
		cin >> cmd;
		if (cmd == 1) {
			int floor, dir, elev;
			cout << "Floor: ";
			cin >> floor;
			cout << "Dir (0 down, 1 still, 2 up): ";
			cin >> dir;
			cout << "Elev (1 is me): ";
			cin >> elev;
			Order order = {floor, (order_direction_t)dir, elev};
			my_manager.addOrder(order);
		}
		else if (cmd == 2) {
			int floor, dir;
			cout << "Choose your floor: ";
			cin >> floor;
			cout << "Choose your dir (0 down, 2 up): ";
			cin >> dir;
			int bfloor = my_manager.getFloorWithLowestCost(floor, (order_direction_t)dir);
			cout << "YOUR BEST FLOOR: " << bfloor << "\n\n";
		}
		else if (cmd == 3) {
			my_manager.clear();
			cout << "CLEARED\n";
		}
		else if (cmd == 4) {
			my_manager.printOrders();
		}
		else {
			break;
		}
	}
}