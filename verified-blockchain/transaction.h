/* Definition of a Verified transaction
 * Copyright Kallol Borah 2019
 */

#include "pch.h"
#include "client.h"

using namespace std;

class transaction {

private :

	struct Tx {
		string hash;
		enum transaction_type { debit, credit, block };
		string block;
		dht::InfoHash certifying_peer;
		dht::InfoHash counterparty;
		dht::InfoHash party;
		time_t timestamp;
	};
	
	static VTrie certificationRequests;

public :

	struct certifiedTx {
		std::string type;
		string hash;
		dht::InfoHash counterparty;
		dht::InfoHash party;
		std::string request;
		std::string response;
		bool certification_status;
		std::chrono::system_clock::time_point timestamp;
	};

	struct verifiedTx {
		std::string type;
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

	void out(client::request&);
	void in(client::request&);
	void certify(certifiedTx&);
	void certified(certifiedTx&);
	void verify(verifiedTx&);
	void verified(verifiedTx&);

};
