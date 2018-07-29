#include <iostream>
#include "OrderTracker.hpp"
using namespace std;

int main(){
	OrderTracker tracker;
	tracker.OnInsertOrderRequest(1,'B',10.0,10);
	tracker.dump();
	tracker.OnRequestAcknowledged(1);
	tracker.dump();
	tracker.OnInsertOrderRequest(2,'O',15.0,25);
	tracker.dump();
	tracker.OnRequestAcknowledged(2);
	tracker.dump();
	tracker.onOrderFilled(1,5);
	tracker.dump();
	tracker.onOrderFilled(1,5);
	tracker.dump();
	tracker.OnReplaceOrderRequest(2,3,10);
	tracker.dump();
	tracker.onOrderFilled(2,25);
	tracker.dump();
	tracker.OnRequestRejected(3);
	tracker.dump();
	return 0;
}
