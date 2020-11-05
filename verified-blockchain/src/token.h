/* Definition of the Verified token
 * Copyright Kallol Borah 2020
 */

#include "pch.h"
#include <vtrie.hpp>

using namespace std;

class token {

private:
	
	std::string name;
	
	static VTrie cstore;

	static struct balances {
		long balance;
		std::string currency;
	};

public:

	token(std::string n) {
		name = n;
	}

	bool transfer(std::string&, long&);

	bool transferFrom(std::string&, std::string&, long&);

	long balanceOf(std::string&);

};