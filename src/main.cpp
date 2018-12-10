#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <type_traits> 
#include <cryptopp/eccrypto.h>
#include <cryptopp/hex.h>
#include <cryptopp/oids.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <asio.hpp>

#include "constants.hpp"
#include "crypto.hpp"
#include "util.hpp"
#include "script.hpp"
#include "blocks.hpp"
#include "node.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace CryptoPP;
//using namespace asio;
using asio::ip::tcp;


int main(){
	//Block GenesisBlock({0,{"ici cest limag"},"","",1566862920,42},{});
	auto consoleLogger = spdlog::stdout_color_mt("console");

	AutoSeededRandomPool prng, rrng;

	eCurve::PrivateKey privateKey;
	privateKey.Initialize( prng, ASN1::secp256k1() );
	eCurve::Signer signer(privateKey);

	eCurve::PublicKey publicKey;
	privateKey.MakePublicKey( publicKey );

	const ECP::Point q = publicKey.GetPublicElement();

	std::string message("TEST PLEASE");
	ECDSASignature signature(message, privateKey);
	
	int status;
	status = mkdir(constants::DATA_PATH.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if( status != 0 && errno != EEXIST){
		char buffer[ 256 ];
		std::cout << "Error when creating directory : " << strerror_r( errno, buffer, 256 ) << std::endl;
	}
	else{
		std::cout << "Directory set" << std::endl;
	}

	asio::io_context io_context;
	Node node(io_context, consoleLogger);
	io_context.run();

	return 0;
}
