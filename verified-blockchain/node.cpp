/* Implementation of Verified node
 * Copyright Kallol Borah 2019
 */

#include "pch.h"
#include "node.h"
#include <msgpack.hpp>

dht::DhtRunner node::_n;

void node::start(dht::crypto::Identity key) {

	node::get_node().run(4222, key, true);

	node::get_node().bootstrap("bootstrap.jami.net", "4222");

	std::thread w(export_nodes);
	worker = std::move(w);
}

void node::start() {

	node::get_node().run(4222, dht::crypto::generateIdentity(), true);

	node::get_node().bootstrap("bootstrap.ring.cx", "4222");

	std::thread w(export_nodes);
	worker = std::move(w);
}

void node::stop() {

	assert(worker.joinable());

	worker.join();

	node::get_node().shutdown();

}

void node::export_nodes() {

	using namespace std::literals::chrono_literals;

	msgpack::unpacker up;
	msgpack::object_handle oh;
	
	while (true) {

		while (up.next(oh)) {
			node::nodes = oh.get().as<std::vector<dht::NodeExport>>();
		}

		std::this_thread::sleep_for(300s);
	}

}


