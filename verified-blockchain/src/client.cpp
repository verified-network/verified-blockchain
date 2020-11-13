/* Implementation of Verified client that mediates communication with the local system
 * Copyright Kallol Borah 2019
 * */


#include "pch.h"
#include "node.h"
#include "asset.h"
#include "transaction.h"


std::future<size_t> rtoken;
std::future<size_t> ctoken;
std::future<size_t> vtoken;
std::future<size_t> ptoken;


void client::startListening(dht::InfoHash& userid) {

	user = userid;
	sessions.push_back(userid);

	std::thread r(listenToRequests);
	listener = std::move(r);

	std::thread c(listenToCertificationRequests);
	listener = std::move(c);

	std::thread v(listenToVerifications);
	listener = std::move(v);
}

void client::stopListening(dht::InfoHash& userid) {

	node::get_node().cancelListen(user, std::move(rtoken));
	node::get_node().cancelListen(user, std::move(ctoken));
	node::get_node().cancelListen(user, std::move(vtoken));

}

void client::listenForVerifications(vector<dht::InfoHash>& peers) {

	for (int i = 0; i < peers.size(); i++) {
		std::thread cc(listenToCertifications, peers.at(i));
		listener = std::move(cc);
	}

}

void client::listenToRequests() {
	
	rtoken = node::get_node().listen<request>(user, [](request&& req, bool expired) {
		if (req.type == "request") {
			//req is the incoming request, forward it to verified run time
			//call(req);
		}
		else if (req.type == "response") {
			//req is returned response, forward it to verified run time
			//callback(req);
		}
		
		return true; // keep listening
	});

}

void client::listenToCertificationRequests() {

	ctoken = node::get_node().listen<transaction::certifiedTx>(user, [](transaction::certifiedTx&& req, bool expired) {
		
		if (req.type == "certify") {
			//certification request is received
			transaction t;
			t.certify(req);
		}
		else if (req.type == "certified") {

			if (!req.certification_status) {
				//handle transaction revert by calling back handler
				//revert();
			}
		}
		
		return true; // keep listening
	});

}

void client::listenToCertifications(dht::InfoHash& peer) {

	ptoken = node::get_node().listen<transaction::certifiedTx>(peer, [](transaction::certifiedTx&& req, bool expired) {

		if (req.type == "certified") {

			if (req.certification_status) {
				//for successful certifications, prompt verification by this (listening previous certifying) peer
				transaction t;
				t.verify(req);
			}
		}

		return true; // keep listening
	});

}


void client::listenToVerifications() {

	vtoken = node::get_node().listen<transaction::verifiedTx>(user, [](transaction::verifiedTx&& req, bool expired) {

		if (user == req.party || user == req.counterparty) {
			if (req.verification_status) {
				//store verified transaction in tx store
				std::stringstream scp;
				msgpack::pack(scp, req);
				const char* verifiedTx = scp.str().c_str();
				transaction::transactionStore.Insert(req.txhash, verifiedTx);
			}
			else {
				//handle transaction revert by calling back handler
				//revert();
			}
		}
		else {
			//call verifying peer
			transaction t;
			t.verified(req, user);
		}

		return true; // keep listening
	});

}


VERIFIEDBLOCKCHAIN_API bool client::handleOutboundRequest(client::request& req) {

	asset a;
	if (a.recordRequest(client::user.to_c_str(), req)) {
		transaction t;
		t.out(req);			
		return true;
	}
	else
		return false;
}

VERIFIEDBLOCKCHAIN_API bool client::handleInboundResponse(client::request& resp) {

	asset a;
	if (a.recordRequest(client::user.to_c_str(), resp)) {
		transaction t;
		t.in(resp);
		return true;
	}
	else
		return false;
}

	

