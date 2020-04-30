/* Verified peer
 * Copyright Kallol Borah 2019
 * */

# include "pch.h"
# include <stdio.h>
# include <fstream>
# include <opendht.h>
# include "node.h"
# include "peer.h"


bool peer::logged = false;
dht::crypto::PublicKey peer::public_key;

VERIFIEDBLOCKCHAIN_API int peer::signup(std::string& username, std::string& pwd) {

	dht::InfoHash userid = dht::InfoHash::get(username+pwd);

	dht::crypto::PrivateKey private_key = dht::crypto::PrivateKey::generate();

	peer::public_key = private_key.getPublicKey();

	//dht::crypto::Certificate cert = dht::crypto::Certificate::generate(private_key, userid.toString);

	//dht::crypto::PrivateKey private_key = dht::crypto::PrivateKey::generate(cert.toString);

	return 1;

}


VERIFIEDBLOCKCHAIN_API int peer::signin(std::string& username, std::string& pwd) {

	peer::logged = true;
		
	return 1;
	
}


VERIFIEDBLOCKCHAIN_API int peer::signout() {

	peer::logged = false;

	return 1;

}








