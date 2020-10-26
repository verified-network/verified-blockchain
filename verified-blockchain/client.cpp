/* Implementation of Verified client that mediates communication with the local system
 * Copyright Kallol Borah 2019
 * */

#include "pch.h"
#include "node.h"
#include "client.h"
#include "asset.h"
#include "transaction.h"

dht::InfoHash user;
std::future<size_t> token;

void client::startListening(dht::InfoHash& userid) {

	user = userid;
	sessions.push_back(userid);

	std::thread l(listen);
	listener = std::move(l);
}

void client::stopListening(dht::InfoHash& userid) {

	node::get_node().cancelListen(user, std::move(token));
}

void client::listen() {

	token = node::get_node().listen<request>(user, [](request&& req, bool expired) {
		if (req.type == "request") {
			//req is the incoming request, forward it to verified run time
			//wait for result encoded in request data structure and updated peer to req.SendTo
			//then forward that for transaction response
			transaction t;
			t.in(req);
		}
		else if (req.type == "response") {
			//req is returned response, forward it to verified run time
			//then forward that for transaction certification
			transaction t;
			t.in(req);
		}
		else if (req.type == "certify") {

		}
		else if (req.type == "verify") {

		}
		return true; // keep listening
	});

}

void client::handleInboundResponse(request& req) {


}

extern "C" {
	
	VERIFIEDBLOCKCHAIN_API bool handleOutboundRequest(client::request& req) {

		asset a;
		if (a.recordRequest(user.to_c_str(), req)) {
			transaction t;
			t.out(req);			
			return true;
		}
		else
			return false;
	}

	VERIFIEDBLOCKCHAIN_API bool registerContract(std::string& contractId) {
		//contract id is container bundle/image hash
		return true;
	}

}


