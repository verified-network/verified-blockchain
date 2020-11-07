/* Definition of Verified client that mediates communication with the local system 
 * Copyright Kallol Borah 2019
 * */

#define VERIFIEDBLOCKCHAIN_API __declspec(dllexport)
#include "pch.h"

using namespace std::literals::chrono_literals;

class client {

private:

	std::thread listener;

	static std::vector<dht::InfoHash> sessions;

public:

	static dht::InfoHash user;

	struct request {
		std::string root;
		std::string type;
		std::vector<dht::InfoHash> verifyingPeers;
		dht::InfoHash sendTo;
		dht::InfoHash from;
		std::string contractId;
		std::string encodedRequest; //with return type, function name, params
		std::string encodedResponse;
		dht::NodeExport requestorPeer;
		dht::NodeExport responderPeer;
	};

	void startListening(dht::InfoHash&);

	void stopListening(dht::InfoHash&);

	static void listenToRequests();

	static void listenToCertificationRequests();

	void listenForVerifications(std::vector<dht::InfoHash>&);

	static void listenToCertifications(dht::InfoHash&);

	static void listenToVerifications();

	VERIFIEDBLOCKCHAIN_API bool handleOutboundRequest(client::request&);

	VERIFIEDBLOCKCHAIN_API bool handleInboundResponse(client::request&);

};
		
		