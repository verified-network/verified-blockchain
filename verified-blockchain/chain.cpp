/* Implementation of the chain of blocks on the Verified network
 * Copyright Kallol Borah 2019
 */

#include "chain.h"

chain::chain(){

	blockchain.emplace_back(block(0, "genesis block"));

}


void chain::addBlock(block b){

	b.prevHash = getLastBlock().getHash();
	b.calculateHash();
	blockchain.push_back(b);

}


block chain::getLastBlock()const{

	return blockchain.back();

}





