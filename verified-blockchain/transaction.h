/* Definition of a Verified transaction
 * Copyright Kallol Borah 2019
 */

#include "pch.h"
#include "client.h"

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

	void out(client::request&);
	void in(client::request&);
	void certify(client::request&);
	void verify(client::request&);

};
