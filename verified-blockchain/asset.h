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
			const char* peer;
			double value;
			const char* currency;
			enum transaction_type { debit, credit, block };
			const char* contract_id;
			const char* request;
			const char* response;
			char* transaction_status;
		};

		static VTrie astore;

		struct balances {
			double balance;
			const char* currency;
		};

		static VTrie cstore;

public :
		balances canDebit(std::string&);
		bool recordRequest(std::string&, client::request&);
};


