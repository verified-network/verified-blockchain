/* Implementation of the chain of blocks on the Verified network
 * Copyright Kallol Borah 2019
 */

#include "pch.h"
#include "chain.h"

chain::chain(){

	auto root = blockchain.GetRoot();
	if (root == NULL) {
		blockchainNode n;
		n.index = 0;
		n.previous = dht::InfoHash("verified network root"); //to do : we need replace with verified network root
	}

}


void chain::addBlock(block b){

	string blockhash = b.calculateHash();

	//encode block and add to blockchain 
	std::stringstream s;
	msgpack::pack(s, b);
	const char* chain = s.str().c_str();
	blockchain.Insert(blockhash, chain);

}


int chain::getSize() {

	return blockchain.size();

}





