/* Implementation of Verified client that mediates communication with the local system
 * Copyright Kallol Borah 2019
 * */

#include "pch.h"
#include "node.h"
#include "client.h"
#include "asset.h"


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
		//req is the incoming request, call the verified run time here
		return true; // keep listening
	});

}

extern "C" {
	
	VERIFIEDBLOCKCHAIN_API bool handleOutboundRequest(client::request& req) {

		asset a;
		a.recordRequest(user.to_c_str(), req);
		return true;
	}

	VERIFIEDBLOCKCHAIN_API bool registerContract(std::string& contractId) {

		return true;
	}

}


