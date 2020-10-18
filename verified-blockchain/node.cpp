/* Implementation of Verified node
 * Copyright Kallol Borah 2019
 */

#include "pch.h"
#include "node.h"

dht::DhtRunner node::_n;

void node::start(dht::crypto::Identity key) {

	node::get_node().run(4222, key, true);

	node::get_node().bootstrap("bootstrap.jami.net", "4222");
}

void node::start() {

	node::get_node().run(4222, dht::crypto::generateIdentity(), true);

	node::get_node().bootstrap("bootstrap.ring.cx", "4222");
	
}

void node::stop() {

	node::get_node().shutdown();

}


