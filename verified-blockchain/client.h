/* Definition of Verified client that mediates communication with the local system 
 * Copyright Kallol Borah 2019
 * */

#define VERIFIEDBLOCKCHAIN_API __declspec(dllexport)
#include "pch.h"

class client {

private :

	std::thread listener;

	static std::vector<dht::InfoHash> sessions;

public :

	struct request {


	};

	void startListening(dht::InfoHash&);

	void stopListening(dht::InfoHash&);

	static void listen();

	void inboundRequestCallback(std::vector<request>);

	VERIFIEDBLOCKCHAIN_API bool handleOutboundRequest(std::vector<request>);

	VERIFIEDBLOCKCHAIN_API bool registerContract(std::string&);

};