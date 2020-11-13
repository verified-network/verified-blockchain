/* verified.i for SWIG */
%module verified
%{
# include "../../../src/peer.h"
# include "../../../src/client.h"
%}
%include <windows.i>

class peer {
	public:
		int signup(std::string&, std::string&);
		std::string signin(std::string&, std::string&);
		int signout(void);
};

class client {
	public:
		struct request {
			std::string root;
			std::string type;
			std::vector<dht::InfoHash> verifyingPeers;
			dht::InfoHash sendTo;
			dht::InfoHash from;
			std::string contractId;
			std::string encodedRequest; 
			std::string encodedResponse;
			dht::NodeExport requestorPeer;
			dht::NodeExport responderPeer;
		};
		bool handleOutboundRequest(client::request&);
		bool handleInboundResponse(client::request&);
};


