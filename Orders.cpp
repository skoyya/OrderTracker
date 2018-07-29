#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <string>
#include "Orders.hpp"
using namespace std;

void Orders::insert(int id, bool isBuy, double price , int qty){
		m_orders[id] = make_shared<Order>(id,isBuy,price,qty);
		if(isBuy){
			m_max_bid += (price*qty);
		}else {
			m_max_offer += (price*qty);
		}
}

void Orders::reject(int id){
	auto it = m_orders.find(id);
	if(it != m_orders.end() ){
		if (it->second->status() == REPLACE)
		{
			if(it->second->isBuy()){
				m_max_bid -= (it->second->price()*it->second->quantity());
			}else{
				m_max_offer -= (it->second->price()*it->second->quantity());
			}
			return;
		}
		//Filled can not be replaced
		if(it->second->status() == FILLED ) return;

		double vol = (it->second->price()*it->second->quantity());
		if(it->second->isBuy()){
			m_max_bid -= vol;
			if(it->second->status() == CONF) {
				m_bid -= vol;
			}
		}else{
			m_max_offer -= vol;
			if(it->second->status() == CONF) {
				m_offer -= vol;
			}
		}
	}

	m_orders.erase(it);
}

void Orders::acknowledge(int id) {
	auto it = m_orders.find(id);
	if(it != m_orders.end() ){
		//only new orders or replace can be acknowledged
		double vol = (it->second->price()*it->second->quantity());
		if(it->second->status() == NEW ){
			if(it->second->isBuy()){
				m_min_bid += vol;
				m_bid += vol;

			}else{
				m_min_offer += vol;
				m_offer += vol;
			}

			it->second->setStatus(CONF);
		}
		if (it->second->status() == REPLACE ) {
			auto it2 = m_orders.find(it->second->id());//get older id to be replaced
			if (it2 != m_orders.end())
			{
				reject(it2->second->id()); //reject old order id
				it->second->setStatus(NEW); //change the replace order to new and conform it
				it->second->setId(id); // now order will have its own id
				it->second->setQuantity(it->second->quantity()+it2->second->quantity());
				acknowledge(id);
			}
		}
	}
}

void Orders::replace(int oldId, int newId, int deltaQuantity){
	auto it = m_orders.find(oldId);
	if (it != m_orders.end() ){
		if(it->second->status() == CONF){  // only conformed orders can be replaced
		auto replaceOrder = make_shared<Order>(oldId,it->second->isBuy(),deltaQuantity,it->second->price());
		if(it->second->isBuy())
		{
		   	m_max_bid += deltaQuantity* it->second->price();
		}else{
		   	m_max_offer += deltaQuantity* it->second->price();
		}
		replaceOrder->setStatus(REPLACE);
			m_orders[newId] = replaceOrder;
		}
	}
}

void Orders::fill(int id, int quantity){
	auto it = m_orders.find(id);
	if (it != m_orders.end() ){
		if( quantity > it->second->quantity()) return; //Exception

		double vol = quantity * it->second->price();
		it->second->setQuantity(it->second->quantity() - quantity);
		if(it->second->isBuy()){
			m_netFilledQuntity += quantity;
			m_bid -= vol;
			m_max_bid -= vol;
			m_min_bid -= vol;
		}else{
			m_netFilledQuntity -= quantity;
			m_offer -= vol;
			m_max_offer -= vol;
			m_min_offer -= vol;
		}
	}
	//for better memory usage, filled orders can be deleted from map
}

ostream& operator<<(ostream& os, const Orders& orders){
	for( auto it = orders.m_orders.begin(); it != orders.m_orders.end(); ++it) {
		
	}
	os<<left<< setw(15)<<"MIN BID" \
		<<left<< setw(15)<<"MAX BID"  \
		<<left<< setw(15)<< "MIN OFFER"  \
		<<left<< setw(15)<<"MAX OFFER"  \
		<<left<< setw(15)<<"BID"  \
		<<left<< setw(15)<<"OFFER" \
		<<left<< setw(15)<<"NFQ"\
		<<left<< setw(15)<<endl; 
	os<<left<< setw(15)<<orders.m_min_bid \
		<<left<< setw(15)<<orders.m_max_bid \
		<<left<< setw(15)<<orders.m_min_offer \
		<<left<< setw(15)<<orders.m_max_offer \
		<<left<< setw(15)<<orders.m_bid\
		<<left<< setw(15)<<orders.m_offer\
		<<left<< setw(15)<<orders.m_netFilledQuntity \
		<<left<< setw(15)<<endl;

	return os;
}
