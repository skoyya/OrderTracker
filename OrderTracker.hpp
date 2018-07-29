#include <iostream>
#include "Interface.hpp"
#include "Orders.hpp"

using namespace std;

class OrderTracker : public Interface {
	Orders m_order_book;

	public:
		OrderTracker(){
		}
		void OnInsertOrderRequest(int id, char side, double price, int quantity){
			m_order_book.insert(id,side=='B',price,quantity);
		}

		void OnRequestRejected(int id){
			m_order_book.reject(id);
		}

		void OnRequestAcknowledged(int id){
			m_order_book.acknowledge(id);
		}

		void OnReplaceOrderRequest(int oldId, int newId, int deltaQuantity){
			m_order_book.replace(oldId, newId, deltaQuantity);
		}

		void onOrderFilled(int id, int qtyFilled){
			m_order_book.fill(id,qtyFilled);
		}

		void dump(){
			cout<<m_order_book<<endl;
		}
};
