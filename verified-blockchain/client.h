/* Definition of Verified client that mediates communication with the local system 
 * Copyright Kallol Borah 2019
 * */

#include "pch.h"

class client {

private :

	struct request {


	};

	std::thread listener;

	static std::vector<std::string> sessions;

public :

	static void startListening(std::string&);

	static void stopListening(std::string&);

	void inboundRequestCallback(std::vector<request>);

	void handleOutboundRequest(std::vector<request>);

};