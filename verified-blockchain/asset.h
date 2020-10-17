/* Definition of the Verified asset model
 * Copyright Kallol Borah 2019
 */

#pragma once
#include<string>
#include<opendht.h>

using namespace std;

class asset {

private :
		struct assets {
			dht::InfoHash peer;
			int value;
			int balance;
			const char* currency;
			enum transaction_type { debit, credit, block };
			const char* contract_id;
			const char* request;
			const char* response;
			char* transaction_status;
		};

public :
		bool canDebit(dht::InfoHash peer);
		int checkBalance(dht::InfoHash peer);
};


