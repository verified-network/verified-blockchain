/* Definition of a Verified transaction
 * Copyright Kallol Borah 2019
 */

#include "pch.h"
#include "client.h"

using namespace std;

class transaction {

private :

	static VTrie certificationRequests;

	static VTrie verifiedTransactions;

public :

	struct certifiedTx {
		std::string root;
		std::string type;
		std::string hash;
		dht::InfoHash counterparty;
		dht::InfoHash party;
		std::string request;
		std::string response;
		bool certification_status;
		std::chrono::system_clock::time_point timestamp;
	};

	struct verifiedTx {
		std::string root;
		std::string txhash;
		std::string type;
		dht::InfoHash certifying_peer;
		dht::InfoHash party;
		dht::InfoHash counterparty;
		std::chrono::system_clock::time_point timestamp;
	};

	void out(client::request&);
	void in(client::request&);
	void certify(certifiedTx&);
	void verify(verifiedTx&);
	void verified(verifiedTx&);

};
