/* Implementation of a Verified transaction
 * Copyright Kallol Borah 2019
 */


#include "pch.h"
#include "transaction.h"
#include "node.h"
#include <msgpack.h>

using namespace std;

void transaction::out(client::request& req) {

	using namespace std::literals::chrono_literals;

	std::vector<dht::NodeExport> nodes = node::get_nodes();
	dht::NodeExport peer;
	std::chrono::seconds life;
	
	//export nodes for this peer
	if (nodes.size() > 0) {
		//if more than one neighbour nodes exist for this peer, then find the oldest living neighbour
		for (int i = 0; i < nodes.size(); i++) {
			dht::NodeExport this_peer = nodes.at(i);
			std::chrono::seconds peer_life;
			//peer_life = peer.lifetime; //to do : add lifetime variable to NodeExport struct in OpenDht
			if (peer_life > life) {
				life = peer_life;
				peer = this_peer;
			}
		}

	}
	//else, use the Verified Network root as neighbour
	else 
		peer = dht::NodeExport{
			//add Verified Network root node
		};

	req.peer = peer;
	req.life = life;
	req.type = "request";

	//encode request
	std::stringstream ss;
	msgpack::pack(ss, req);
	const char* serializedRequest = ss.str().c_str();

	node::get_node().put(
		//put request on network at peer sentTo
		req.sendTo,
		dht::Value((const uint8_t*)serializedRequest, std::strlen(serializedRequest))
	);

}

void transaction::in(client::request& req) {

	using namespace std::literals::chrono_literals;

	if (req.type == "request") {

		std::vector<dht::NodeExport> nodes = node::get_nodes();
		dht::NodeExport peer = req.peer;
		std::chrono::seconds life = req.life;

		//export nodes for this peer
		if (nodes.size() > 0) {
			//if more than one neighbour nodes exist for this peer, then find the oldest living neighbour
			for (int i = 0; i < nodes.size(); i++) {
				dht::NodeExport this_peer = nodes.at(i);
				std::chrono::seconds peer_life;
				//peer_life = peer.lifetime; //to do : add lifetime variable to NodeExport struct in OpenDht
				if (peer_life > req.life) {
					life = peer_life;
					peer = this_peer;
				}
			}

		}

		req.peer = peer;
		req.life = life;
		req.type = "response";

		//encode request
		std::stringstream ss;
		msgpack::pack(ss, req);
		const char* serializedRequest = ss.str().c_str();

		node::get_node().put(
			//put request on network at requestor peer sentTo
			req.sendTo,
			dht::Value((const uint8_t*)serializedRequest, std::strlen(serializedRequest))
		);

	}



}

void transaction::certify(client::request& req) {



}

void transaction::verify(client::request& req) {



}