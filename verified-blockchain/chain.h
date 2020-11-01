/* Definition of a chain of blocks on the Verified network
 * Copyright Kallol Borah 2019
 */

#include "pch.h"
#include "block.h"
#include <vtrie.hpp>
#include <msgpack.h>

using namespace std;

class chain {

	private :

		uint32_t difficulty;
		
		block getLastBlock() const;

	public :

		static VTrie blockchain;

		struct blockchainNode {
			dht::InfoHash previous;
			int index;
		};
		
		chain();
	
		void addBlock(block blck);

		int getSize();

};


