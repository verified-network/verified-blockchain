/* Definition of a chain of blocks on the Verified network
 * Copyright Kallol Borah 2019
 */

#pragma once
#include <cstdint>
#include <vector>
#include "block.h"

using namespace std;

class chain {

	private :

		uint32_t difficulty;
		vector<block> blockchain;

		block getLastBlock() const;

	public :
		
		chain();
	
		void addBlock(block blck);

};


