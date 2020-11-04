/* Definition of a block on the Verified network
 * Copyright Kallol Borah 2019
 */

#include "pch.h"

using namespace std;

class block{

	private :

		uint32_t index;
		string data;
		string hash;
		time_t current_time;

	public :

		string prevHash;
		
		block(uint32_t indexIn, const string &dataIn, dht::InfoHash);
		
		string calculateHash();

		string getHash();

};


