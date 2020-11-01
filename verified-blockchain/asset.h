/* Definition of the Verified asset model
 * Copyright Kallol Borah 2019
 */

#include "pch.h"
#include "client.h"
#include <vtrie.hpp>

using namespace std;

class asset {

private :
		struct assets {
			std::string requestor;
			std::string responder;
			double value;
			const char* currency;
			enum transaction_type { debit, credit, block };
			std::string contract_id;
			std::string request;
			std::string response;
			bool transaction_status;
		};

		static VTrie astore;

		struct balances {
			float balance;
			const char* currency;
		};

		static VTrie cstore;

public :
		
		balances canDebit(std::string&);

		bool recordRequest(std::string&&, client::request&);
		
		bool recordResponse(std::string&, client::request&);
};


