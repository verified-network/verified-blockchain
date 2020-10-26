/* Definition of Verified client that mediates communication with the local system 
 * Copyright Kallol Borah 2019
 * */

#define VERIFIEDBLOCKCHAIN_API __declspec(dllexport)
#include "pch.h"

using namespace std::literals::chrono_literals;

class client {

private :

	std::thread listener;

	static std::vector<dht::InfoHash> sessions;

public :

	struct request {
		std::string type;
		dht::InfoHash sendTo;
		dht::InfoHash from;
		std::string contractId;
		std::string encodedRequest; //with return type, function name, params
		std::string encodedResponse;
		dht::NodeExport peer;
		std::chrono::seconds life;
	};

	void startListening(dht::InfoHash&);

	void stopListening(dht::InfoHash&);

	static void listen();

	void handleInboundResponse(request&);

	VERIFIEDBLOCKCHAIN_API bool handleOutboundRequest(request&);

	VERIFIEDBLOCKCHAIN_API bool registerContract(std::string&);

};