/* Definition of Node
 * Copyright Kallol Borah 2019
 */

#include "pch.h"

class node {

	private:

		static dht::DhtRunner _n;

	public:	

		void start(dht::crypto::Identity);

		void start();

		void stop();

		static dht::DhtRunner& get_node() {

			return _n;

		}

};

