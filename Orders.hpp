#include <iostream>
#include <unordered_map>
using namespace std;

enum Status { NEW, CONF, FILLED, REPLACE};
class Order {
	int m_id;
	bool m_buy;
	int  m_qty;
	double m_price;
	Status m_status;
	public:
	Order(int id, bool buy, const double& price, const int& qty):m_id(id),m_buy(buy),m_qty(qty),m_price(price),m_status(NEW)
	{}
	
	int    id()		  const { return m_id;}
	bool   isBuy()    const { return m_buy;}
	int    quantity() const { return m_qty;}
	double price()    const { return m_price;}
	Status status()   const { return m_status;}

	void setStatus(Status status) { m_status = status; }
	void setId(int id) { m_id = id; }
	void setQuantity(int qty) { m_qty = qty; }
};

class Orders {
	unordered_map<int,shared_ptr<Order> >  m_orders;
	double m_min_bid;
	double m_max_bid;
	double m_min_offer;
	double m_max_offer;
	double m_bid;
	double m_offer;
	int	   m_netFilledQuntity;
	public:
	Orders():m_min_bid(0),m_max_bid(0),m_min_offer(0),m_max_offer(0),m_bid(0),m_offer(0),m_netFilledQuntity(0){}

	void insert(int id, bool isBuy, double price, int qty);
	void reject(int id);
	void acknowledge(int id) ;
	void replace(int oldId, int newId, int deltaQuantity);
	void fill(int id, int quantity);
	friend ostream& operator<<(ostream& os, const Orders& orders); 
};
