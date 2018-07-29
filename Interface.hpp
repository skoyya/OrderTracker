class Interface {
	public:
		virtual void OnInsertOrderRequest(int id, char side, double price, int quantity) = 0;
	    virtual void OnRequestRejected(int id)	=0;	
	    virtual void OnRequestAcknowledged(int id)	=0;
		virtual void OnReplaceOrderRequest(int oldId, int newId, int deltaQuantity) = 0;
		virtual void onOrderFilled(int id, int qtyFilled) = 0;

		virtual ~Interface() {} 
};
