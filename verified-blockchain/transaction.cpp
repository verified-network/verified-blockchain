/* Implementation of a Verified transaction
 * Copyright Kallol Borah 2019
 */


#include "pch.h"
#include "transaction.h"
#include "node.h"
#include "sha256.h"
#include <msgpack.h>
#include <vtrie.hpp>

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

	req.root = transaction::verifiedTransactions.GetRoot();
	req.requestorPeer = peer;
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
		dht::NodeExport peer = req.requestorPeer;
		std::chrono::seconds life; //= peer.life; //this is to be added

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
		
		req.root = transaction::verifiedTransactions.GetRoot();
		req.responderPeer = peer;
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

	//for all outgoing and incoming responses, transaction certification requests are sent

	dht::InfoHash certifying_peer;
	certifiedTx c;

	if (req.requestorPeer.sslen > req.responderPeer.sslen) {
		certifying_peer = req.requestorPeer.id;
	}
	else {
		certifying_peer = req.responderPeer.id;
	}

	c.party = req.from;
	c.counterparty = req.sendTo;
	c.hash = sha256(req.encodedRequest+req.encodedResponse);
	c.request = sha256(req.encodedRequest);
	c.response = sha256(req.encodedResponse);
	c.root = req.root;

	//encode request
	std::stringstream s;
	msgpack::pack(s, c);
	const char* certificationRequest = s.str().c_str();

	node::get_node().put(
		//put request on network for certifying peer
		certifying_peer,
		dht::Value((const uint8_t*)certificationRequest, std::strlen(certificationRequest))
	);

}

void transaction::certify(transaction::certifiedTx& req) {

	std::stringstream ss;
	msgpack::pack(ss, req.hash);

	auto result = certificationRequests.Select(sha256(ss.str()));

	if (result == NULL) {
		//if no counter party has sent this request before, store the certification request in store
		std::stringstream ss;
		msgpack::pack(ss, req);
		if (!certificationRequests.Insert(req.hash, ss.str())) {

			//update certification request status
			req.certification_status = false;

			//if certification request can't be stored on certifying peer for some reason
			std::stringstream s;
			msgpack::pack(s, req);
			const char* certificationRequest = s.str().c_str();

			node::get_node().put(
				//let party know
				req.party,
				dht::Value((const uint8_t*)certificationRequest, std::strlen(certificationRequest))
			);

			node::get_node().put(
				//let counterparty also know
				req.counterparty,
				dht::Value((const uint8_t*)certificationRequest, std::strlen(certificationRequest))
			);

		}
	}
	else {
		//else, retrieve stored certification request 
		msgpack::unpacked unp;
		msgpack::unpack(unp, result.str().data(), result.str().size());
		msgpack::object res = unp.get();
		certifiedTx requests = res.as<certifiedTx>();

		if (requests.request == req.request && requests.response == req.response) {

			//update certification status as successful
			req.certification_status = true;
			req.timestamp = std::chrono::system_clock::now();

			requests.certification_status = true;
			requests.timestamp = req.timestamp;
			
			//update transaction store
			std::stringstream scp;
			msgpack::pack(scp, requests);
			const char* certificationRequest = scp.str().c_str();
			certificationRequests.Update(requests.hash, certificationRequest);

			//insert new request into transaction store
			std::stringstream s;
			msgpack::pack(s, req);
			const char* certificationRequest = s.str().c_str();
			certificationRequests.Insert(req.hash, certificationRequest);

			//send out verification requests
			const char* rootp = req.root.c_str();
			node::get_node().put(
				//let party know
				req.party,
				dht::Value((const uint8_t*)rootp, std::strlen(rootp))
			);

			const char* rootcp = requests.root.c_str();
			node::get_node().put(
				//let counterparty also know
				requests.counterparty,
				dht::Value((const uint8_t*)rootcp, std::strlen(rootcp))
			);


		}
		else {

			//update certification status as unsuccessful
			req.certification_status = false;
			req.timestamp = std::chrono::system_clock::now();

			//update transaction store
			std::stringstream s;
			msgpack::pack(s, req);
			const char* certificationRequest = s.str().c_str();
			certificationRequests.Update(req.hash, certificationRequest);

			node::get_node().put(
				//let party know
				req.party,
				dht::Value((const uint8_t*)certificationRequest, std::strlen(certificationRequest))
			);

			std::stringstream scp;
			msgpack::pack(scp, requests);
			const char* certificationRequest = scp.str().c_str();

			node::get_node().put(
				//let counterparty also know
				requests.counterparty,
				dht::Value((const uint8_t*)certificationRequest, std::strlen(certificationRequest))
			);
		}
	}

}

void transaction::verify(transaction::verifiedTx& req) {



}

void transaction::verified(transaction::verifiedTx& req) {



}