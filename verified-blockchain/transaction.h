/* Definition of a Verified transaction
 * Copyright Kallol Borah 2019
 */

#pragma once
#include<string>
#include <cstdint>
#include<opendht.h>

using namespace std;

class transaction {

private :
		struct transactions {
			string id;
			enum transaction_type { debit, credit, block };
			string block;
			dht::InfoHash certifying_peer;
			dht::InfoHash counterparty;
			dht::InfoHash party;
			time_t timestamp;
		};

		struct verified {
			dht::InfoHash party;
			dht::InfoHash counterparty;
			string request;
			string response;
			dht::InfoHash previous_peer;
			dht::InfoHash next_peer;
			const char* transaction_status;
			string transaction;
			string block;
			time_t timestamp;
		};

public :


};
