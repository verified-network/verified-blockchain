/* Defines Verified peer
 * Author Kallol Borah
 * Copyright 2019
*/

#define VERIFIEDBLOCKCHAIN_API __declspec(dllexport)

#include "pch.h"

class peer {

	private:

		dht::InfoHash userid;

	public:

		static bool logged;

		static dht::crypto::PublicKey public_key;

		VERIFIEDBLOCKCHAIN_API int signup(std::string&, std::string&);

		VERIFIEDBLOCKCHAIN_API std::string signin(std::string&, std::string&);

		VERIFIEDBLOCKCHAIN_API int signout(void);

};
