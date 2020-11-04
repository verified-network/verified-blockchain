/* Verified peer
 * Copyright Kallol Borah 2019
 * */

#include "pch.h"
#include "node.h"
#include "peer.h"
#include "client.h"

extern "C" {
	
	bool peer::logged = false;
	dht::crypto::PublicKey peer::public_key;

	VERIFIEDBLOCKCHAIN_API int peer::signup(std::string& username, std::string& pwd) {

		if (!node::get_node().isRunning()) {
			node n;
			n.start();
		}

		userid = dht::InfoHash::get(username + pwd);

		std::vector<uint8_t> user{ userid.begin(), userid.end() };

		dht::crypto::PrivateKey private_key(user, pwd);

		peer::public_key = private_key.getPublicKey();

		//encrypted userid has to be persisted as an asset on network, so put function below has to be wrapped inside a verified transaction
		node::get_node().putSigned(userid.to_c_str(), peer::public_key.encrypt(user));

		return 1;
	}


	VERIFIEDBLOCKCHAIN_API dht::InfoHash peer::signin(std::string& username, std::string& pwd) {

		if (!node::get_node().isRunning()) {
			node n;
			n.start();
		}

		userid = dht::InfoHash::get(username + pwd);

		std::vector<uint8_t> user{ userid.begin(), userid.end() };

		dht::crypto::PrivateKey private_key(user, pwd);

		peer::public_key = private_key.getPublicKey();

		//get function below has to be wrapped inside a verified transaction as encrypted userid is persisted as an asset on network
		node::get_node().get(userid.to_c_str(), [&](const std::vector<std::shared_ptr<dht::Value>>& values) {

			std::vector<uint8_t> decrypted;

			for (const auto& value : values) {
				std::vector<uint8_t> val{ value->data.begin(), value->data.end() };
				decrypted = private_key.decrypt(val);
				if (user == decrypted) {
					peer::logged = true;
					client c; 
					c.startListening(userid);
					return 1;
				}
				else
					return 0;
			}

			});

		return userid;

	}


	VERIFIEDBLOCKCHAIN_API int peer::signout() {

		node::get_node().join();

		node::get_node().shutdown();

		peer::logged = false;

		client c;
		c.stopListening(userid);

		return 1;

	}

}






