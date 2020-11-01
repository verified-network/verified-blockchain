/* Definition of a Verified transaction
 * Copyright Kallol Borah 2019
 */

#include "pch.h"
#include "client.h"
#include <vtrie.hpp>

using namespace std;

class transaction {

private :

	static VTrie certificationRequests;

	static VTrie verifiedTransactions;

public :

	static VTrie transactionStore;

	struct certifiedTx {
		std::string root;
		std::string type;
		std::string hash;
		dht::InfoHash certifying_peer;
		vector<dht::InfoHash> previous_verifyingPeers;
		dht::InfoHash counterparty;
		dht::InfoHash party;
		std::string request;
		std::string response;
		bool certification_status;
		std::chrono::system_clock::time_point timestamp;
	};

	struct verifiedTx {
		std::string txhash;
		dht::InfoHash verifying_peer;
		dht::InfoHash party;
		dht::InfoHash counterparty;
		std::chrono::system_clock::time_point timestamp;
		bool verification_status;
	};

	void out(client::request&);

	void in(client::request&);
	
	void certify(certifiedTx&);
	
	void verify(certifiedTx&);
	
	void verified(verifiedTx&, const dht::InfoHash&);

};
