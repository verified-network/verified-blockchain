/* Definition of Verified client that mediates communication with the local system 
 * Copyright Kallol Borah 2019
 * */

#include "pch.h"

class client {

private :

	struct request {


	};

	std::thread listener;

	std::vector<dht::InfoHash> sessions;

public :

	void startListening(dht::InfoHash key);

	void stopListening(dht::InfoHash key);

	void handleInboundRequest(std::vector<request> request);

	void handleOutboundRequest(std::vector<request> request);

};