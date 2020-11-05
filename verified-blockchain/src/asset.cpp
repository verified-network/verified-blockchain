/* Implementation of the Verified asset model
 * Copyright Kallol Borah 2019
 */

#include "pch.h"
#include "asset.h"
#include <vtrie.hpp>
#include <msgpack.hpp>
#include "sha256.h"
#include "resource.h"
#include "token.h"

using namespace std;

bool asset::recordRequest(std::string&& peer, client::request& req) {

	std::stringstream ss;
	msgpack::pack(ss, req.encodedRequest);

	auto result = astore.Select(sha256(ss.str()));
	if (result == NULL) {

		//no request is logged yet, check if enough via balance exists to create request
		token b = token("via");
		if (b.balanceOf(peer) > 0) {

			//then create
			assets a;
			a.request = sha256(ss.str());
			std::stringstream s;
			msgpack::pack(s, req.from);
			a.requestor = s.str();
			a.currency = "via";
			a.contract_id = req.contractId;

			//and store request
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


bool asset::recordResponse(std::string& peer, client::request& req) {

	std::stringstream ss;
	msgpack::pack(ss, req.encodedRequest);

	auto result = astore.Select(sha256(ss.str()));

	if (result != NULL) {

		//request is logged, extract request attributes from result
		msgpack::unpacked unp;
		msgpack::unpack(unp, result.str().data(), result.str().size());
		msgpack::object res = unp.get();
		assets s = res.as<assets>();

		//store response
		std::stringstream st;
		msgpack::pack(st, req.encodedResponse);
		s.response = sha256(st.str());
		msgpack::pack(st, req.from);
		s.responder = st.str();

		//deduct cost of resource consumed
		if (peer == s.responder) {
			resource r;
			token b = token("via");
			long cost = r.calculateCost(s.contract_id);
			if (b.balanceOf(peer) > cost) {
				//cost is shared equally by responder and requestor
				s.value = cost / 2; 
				s.transaction_status = true;
			}
			else {
				//to do : revert transaction at responder's side
				s.value = cost / 2;
				s.transaction_status = false;
			}
		}
		else if (peer == s.requestor) {
			token b = token("via");
			if (b.balanceOf(peer) > s.value) {
				s.transaction_status = true;
			}
			else {
				//to do : revert transaction at requestor's side
				//to do : send message to responder to revert
				s.transaction_status = false;
			}
		}
		else
			return false;

		//and store response
		msgpack::pack(st, s);
		if (astore.Update(sha256(ss.str()), st.str()))
			return true;
		else
			return false;
		
	}
	else
		return false;

}


	

