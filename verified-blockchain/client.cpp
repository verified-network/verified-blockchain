/* Implementation of Verified client that mediates communication with the local system
 * Copyright Kallol Borah 2019
 * */

#include "pch.h"
#include "node.h"
#include "asset.h"
#include "transaction.h"

dht::InfoHash user;
std::future<size_t> rtoken;
std::future<size_t> ctoken;
std::future<size_t> vtoken;

void client::startListening(dht::InfoHash& userid) {

	user = userid;
	sessions.push_back(userid);

	std::thread r(listenToRequests);
	listener = std::move(r);

	std::thread c(listenToCertificationRequests);
	listener = std::move(c);

	std::thread v(listenToVerificationRequests);
	listener = std::move(v);
}

void client::stopListening(dht::InfoHash& userid) {

	node::get_node().cancelListen(user, std::move(rtoken));
	node::get_node().cancelListen(user, std::move(ctoken));
	node::get_node().cancelListen(user, std::move(vtoken));

}

void client::listenToRequests() {
	
	rtoken = node::get_node().listen<request>(user, [](request&& req, bool expired) {
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

			}
		}
		
		return true; // keep listening
	});

}

void client::listenToVerificationRequests() {

	vtoken = node::get_node().listen<transaction::verifiedTx>(user, [](transaction::verifiedTx&& req, bool expired) {

		if (req.type == "verify") {
			//verification request is received
			transaction t;
			t.verify(req);
		}
		else if (req.type == "verified") {
			//verification status is received
			transaction t;
			t.verified(req);
		}

		return true; // keep listening
	});

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


