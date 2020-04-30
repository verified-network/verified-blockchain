/* Defines Verified peer
 * Author Kallol Borah
 * Copyright 2019
*/
#pragma once

#ifdef VERIFIEDBLOCKCHAIN_EXPORTS
#define VERIFIEDBLOCKCHAIN_API __declspec(dllexport)
#else
#define VERIFIEDBLOCKCHAIN_API __declspec(dllimport)
#endif

#include "opendht.h"
#include <string>

class VERIFIEDBLOCKCHAIN_API peer {

	public:

		bool static logged;

		static dht::crypto::PublicKey public_key;

		int signup(std::string&, std::string&);

		int signin(std::string&, std::string&);

		int signout(void);

};
