/* Implementation of the Verified asset model
 * Copyright Kallol Borah 2019
 */

#include "pch.h"
#include "asset.h"
#include <vtrie.hpp>
#include "sha256.h"

using namespace std;

bool asset::recordRequest(std::string& peer, client::request& req) {

	auto result = astore.select(sha256(req));
	if (result == null) {
		
		//no request is logged yet, check if enough via balance exists to create request
		balances b = canDebit(peer);
		if (b.balance>0) {
			
			//then create
			assets a;
			a.request = sha256(req);
			a.peer = peer;
			a.currency = b.currency;
			a.value = 0;
			a.transaction_status = a.debit;
			
			//and store request
			if (astore.Insert(sha256(req), a))
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

balances asset::canDebit(std::string& peer) {

	auto result = cstore.select(peer);
	if (result != null) {
		return (1, "via");
	}
	else
		return (0, "via");
}