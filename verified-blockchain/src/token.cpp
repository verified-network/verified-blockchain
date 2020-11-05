/* Implementation of the Token template
 * Copyright Kallol Borah 2020
 */

#include "pch.h"
#include "token.h"
#include "sha256.h"

bool token::transfer(std::string& to, long& amount) {

	auto result = cstore.Select(to);
	if (result != NULL) {
		//if any token is logged, extract token balances from result
		msgpack::unpacked unp;
		msgpack::unpack(unp, result.str().data(), result.str().size());
		msgpack::object res = unp.get();
		balances b = res.as<balances>();

		//check for token balances
		if (b.currency == name)
			if (b.balance >= amount) {
				b.balance = b.balance + amount;

				std::stringstream st;
				msgpack::pack(st, b);
				if (cstore.Update(sha256(to), st.str()))
					return true;
				else
					return false;
			}
			else
				return false;
		return false;
	}
	else
		//return status
		false;
}

bool token::transferFrom(std::string& from, std::string& to, long& amount) {

	auto result = cstore.Select(from);
	if (result != NULL) {
		//if any token is logged, extract token balances from result
		msgpack::unpacked unp;
		msgpack::unpack(unp, result.str().data(), result.str().size());
		msgpack::object res = unp.get();
		balances b = res.as<balances>();

		//check for token balances
		if (b.currency == name)
			if (b.balance >= amount) {
				transfer(to, amount);
				b.balance = b.balance - amount;

				std::stringstream st;
				msgpack::pack(st, b);
				if (cstore.Update(sha256(from), st.str()))
					return true;
				else
					return false;
			}
			else
				return false;
		return false;
	}
	else
		//return status
		false;
}

long token::balanceOf(std::string& peer) {

	auto result = cstore.Select(peer);
	if (result != NULL) {
		//if any token is logged, extract token balances from result
		msgpack::unpacked unp;
		msgpack::unpack(unp, result.str().data(), result.str().size());
		msgpack::object res = unp.get();
		balances b = res.as<balances>();

		//check for token balances
		if (b.currency == name)
			return b.balance;
	}
	else
		//return nothing
		0;

}