/* Implementation of a block
 * Copyright Kallol Borah 2019
 */

#include "pch.h"
#include "block.h"
#include "sha256.h"
#include <sstream>
#include <ctime>


block::block(uint32_t indexIn, const string &dataIn){

	current_time = time(NULL);

}


string block::getHash(){

	return hash;

}


string block::calculateHash(){

	stringstream ss;
	ss << index << current_time << data << prevHash;
	//hash = sha256(ss.str());
	return hash;

}
