/* Definition of Node
 * Copyright Kallol Borah 2019
 */

#include "pch.h"

using namespace std;

class node {
	
	private:

		static dht::DhtRunner _n;

		std::thread worker;

		static std::vector<dht::NodeExport> nodes;

		static void export_nodes(); 

	public:	

		void start(dht::crypto::Identity);

		void start();

		void stop();

		static dht::DhtRunner& get_node() {

			return _n;

		}

		static std::vector<dht::NodeExport> get_nodes() {

			return nodes;

		}

};

