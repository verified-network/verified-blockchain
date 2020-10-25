/* Implementation of the Verified asset model
 * Copyright Kallol Borah 2019
 */

#include "pch.h"
#include "asset.h"
#include <vtrie.hpp>
#include <msgpack.hpp>
#include "sha256.h"

using namespace std;

bool asset::recordRequest(std::string&& peer, client::request& req) {

	std::stringstream ss;
	msgpack::pack(ss, req);

	auto result = astore.Select(sha256(ss.str()));
	if (result == NULL) {
		
		//no request is logged yet, check if enough via balance exists to create request
		balances b = canDebit(peer);
		if (b.balance>0) {
			
			//then create
			assets a;
			a.request = sha256(ss.str());
			a.requestor = peer;
			a.currency = b.currency;
			a.value = 0;
			a.transaction_status = a.debit;
			
			//and store request
			std::stringstream s;
			msgpack::pack(s, a);
			if (astore.Insert(sha256(ss.str()), s.str()))
				return true;
			else
				return false;
		}
		else
			return false;
	}
	else 
		return false;
}

bool asset::recordResponse(std::string& peer, client::request& req, client::request& resp) {

	std::stringstream ss;
	msgpack::pack(ss, req);

	auto result = astore.Select(sha256(ss.str()));

	if (result != NULL) {

		//request is logged, extract request attributes from result
		assets a;
		msgpack::unpacked unp;
		msgpack::unpack(unp, result.str().data(), result.str().size());
		msgpack::object res = unp.get();
		assets s = res.as<assets>();

		//store response
		std::stringstream st;
		msgpack::pack(st, resp);
		a.response = sha256(st.str());
		a.responder = peer;

		//to stored request parameters
		a.request = s.request;
		a.requestor = s.requestor;
		a.currency = s.currency;
		a.value = s.value;
		a.transaction_status = s.transaction_status;

		//and store response
		std::stringstream ss;
		msgpack::pack(ss, a);
		if (astore.Update(sha256(ss.str()), ss.str()))
			return true;
		else
			return false;
		
	}
	else
		return false;

}

asset::balances asset::canDebit(std::string& peer) {

	auto result = cstore.Select(peer);
	if (result != NULL) {
		//if any asset is logged, extract asset balances from result
		msgpack::unpacked unp;
		msgpack::unpack(unp, result.str().data(), result.str().size());
		msgpack::object res = unp.get();
		balances b = res.as<balances>();

		//check for via balances
		if(b.currency=="via")
			return b;
	}
	else
		//return nothing
		return balances();
}