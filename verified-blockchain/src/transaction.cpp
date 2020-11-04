/* Implementation of a Verified transaction
 * Copyright Kallol Borah 2019
 */


#include "pch.h"
#include "transaction.h"
#include "node.h"
#include "sha256.h"
#include "block.h"
#include "chain.h"
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

	req.root = transaction::transactionStore.GetRoot();
	req.requestorPeer = peer;
	req.type = "request";

	auto result = transaction::transactionStore.GetLast(3);
	msgpack::unpacked unp;
	msgpack::unpack(unp, result.str().data(), result.str().size());
	msgpack::object res = unp.get();
	certifiedTx tx = res.as<certifiedTx>();
	req.verifyingPeers = tx.previous_verifyingPeers;

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
		
		req.root = transaction::transactionStore.GetRoot();
		req.responderPeer = peer;
		req.type = "response";

		auto result = transaction::transactionStore.GetLast(3);
		msgpack::unpacked unp;
		msgpack::unpack(unp, result.str().data(), result.str().size());
		msgpack::object res = unp.get();
		certifiedTx tx = res.as<certifiedTx>();
		req.verifyingPeers = tx.previous_verifyingPeers;

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

	//to do : sslen needs to be replaced with life in ms
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
	c.previous_verifyingPeers = req.verifyingPeers;

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
			req.certifying_peer = client::user;

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
			requests.certification_status = true;
		}
		else {

			//update certification status as unsuccessful
			req.certification_status = false;
			requests.certification_status = false;
		}

		req.timestamp = std::chrono::system_clock::now();
		requests.timestamp = req.timestamp;
		req.certifying_peer = client::user;
		requests.certifying_peer = req.certifying_peer;
					
		//update transaction store
		std::stringstream scp;
		msgpack::pack(scp, requests);
		const char* certificationRequest = scp.str().c_str();
		certificationRequests.Update(requests.hash, certificationRequest);

		node::get_node().put(
			//let party know
			req.party,
			dht::Value((const uint8_t*)certificationRequest, std::strlen(certificationRequest))
		);

		node::get_node().put(
			//let counterparty also know
			requests.counterparty,
			dht::Value((const uint8_t*)certificationRequest, std::strlen(certificationRequest))
		);

		if (req.previous_verifyingPeers.size() == 0 && requests.previous_verifyingPeers.size() == 0
			&& requests.request == req.request && requests.response == req.response) {
			//if previous verifying peers of requestor and responder are not there, then consider transaction verified
			verifiedTx vtx;
			vtx.txhash = req.hash;
			vtx.verifying_peer = client::user;
			vtx.party = req.party;
			vtx.counterparty = req.counterparty;
			vtx.timestamp = req.timestamp;
			vtx.verification_status = true;
			verified(vtx, dht::InfoHash("verified network root")); //to do : we need replace with verified network root
			
		}
		
	}

}

void transaction::verify(transaction::certifiedTx& req) {

	//find out if the requestor's or responder's transaction store root is provable on the blockchain since transaction roots are keys for blocks chained
	auto proof_nodes = chain::blockchain.Prove(chain::blockchain, req.root);
	auto proved = chain::blockchain.VerifyProof(chain::blockchain.GetRoot(), req.root, proof_nodes);

	verifiedTx vtx;
	vtx.txhash = req.hash;
	vtx.verifying_peer = client::user;
	vtx.party = req.party;
	vtx.counterparty = req.counterparty;
	vtx.timestamp = req.timestamp;

	if (!proved) {
		//if transaction root is not provable, update status 
		vtx.verification_status = false;
	}
	else {
		//if proved, transaction is verified successfully
		vtx.verification_status = true;

		//however, if this peer is not in the list of previous verifying peers of the transaction, stop listening for verifications
		bool found = false;
		for (int i = 0; i < req.previous_verifyingPeers.size(); i++) {

			if (client::user == req.previous_verifyingPeers.at(i)) {
				found = true;
			}
		}
		if (!found) {
			//stop client's listener for this party's certifications

		}
	}

	std::stringstream scp;
	msgpack::pack(scp, vtx);
	const char* verificationRequest = scp.str().c_str();

	node::get_node().put(
		//send verification status to the transaction's current certifying peer
		req.certifying_peer,
		dht::Value((const uint8_t*)verificationRequest, std::strlen(verificationRequest))
	);

}

void transaction::verified(transaction::verifiedTx& tx, const dht::InfoHash& verifier) {

	//encode verified transaction
	std::stringstream scp;
	msgpack::pack(scp, tx);
	const char* verifiedTx = scp.str().c_str();
	
	//add verified transaction to blockchain
	chain c;
	block b(c.getSize(), scp.str().c_str(), verifier);
	c.addBlock(b);
	
	node::get_node().put(
		//let party know
		tx.party,
		dht::Value((const uint8_t*)verifiedTx, std::strlen(verifiedTx))
	);

	node::get_node().put(
		//let counterparty also know
		tx.counterparty,
		dht::Value((const uint8_t*)verifiedTx, std::strlen(verifiedTx))
	);

	//if transaction is verified successfully, listen to transaction's requestor and responder for future verifications
	if (tx.verification_status) {
		vector<dht::InfoHash> peers;
		peers.push_back(tx.party);
		peers.push_back(tx.counterparty);
		client c;
		c.listenForVerifications(peers);
	}

}