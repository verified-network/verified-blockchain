/* Implementation of Verified client that mediates communication with the local system
 * Copyright Kallol Borah 2019
 * */

#include "pch.h"
#include "node.h"
#include "client.h"

dht::InfoHash user;


void client::startListening(dht::InfoHash userid) {

	user = userid;
	sessions.push_back(userid);

	std::thread l(listen);
	listener = std::move(l);
}

void client::stopListening(dht::InfoHash userid) {

	node::get_node().cancelListen(user, token);
}

void listen() {

	auto token = node::get_node().listen<request>(user, [](request&& values, bool expired) {
		for (const auto& v : values) {
			//v is the incoming request, call the container 

		}
	}

}



