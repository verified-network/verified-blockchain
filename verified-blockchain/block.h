/* Definition of a block on the Verified network
 * Copyright Kallol Borah 2019
 */
#pragma once
#include <cstdint>
#include <iostream>

using namespace std;

class block{

	private :

		uint32_t index;
		string data;
		string hash;
		time_t current_time;


	public :

		string prevHash;
		
		block(uint32_t indexIn, const string &dataIn);
		
		string calculateHash();

		string getHash();

};


