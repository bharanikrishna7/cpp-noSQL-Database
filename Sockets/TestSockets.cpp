#ifdef TEST_SOCKETS

#include <thread>
#include <iostream>

#include "Client.h"
#include "Server.h"
#include "../Utilities/Utilities.h"

/* Include Utilities Namespace for StringHelper Functions */
using namespace Utilities;

/// <summary>
/// Simple Implementation of Server Class for Testing Purpose.
/// </summary>
class ServerApp : public Server {
protected:
	/// <summary>
	/// Function to Process Client's Request and Return Response.
	/// </summary>
	/// <param name="clientSocket">Client's Socket</param>
	/// <param name="buffer">Client Request</param>
	/// <param name="bufferSize">Size of Request Buffer</param>
	void response(SOCKET clientSocket, std::string buffer, int bufferSize) {
		// echo message back to client
		std::cout << " SERVER RESPONSE : > " << buffer;
		send(clientSocket, buffer.c_str(), bufferSize + 1, 0);
	}

	/// <summary>
	/// Function to Process Client's Request and Broadcasts Response
	/// to all the Clients.
	/// </summary>
	/// <param name="buffer">Client Request</param>
	/// <param name="bufferSize">Size of Request Buffer</param>
	void responseBroadcast(std::string buffer, int bufferSize) {
		// do nothing
		// doesnot support broadcast
	}
public:
	/// <summary>
	/// Constructor for the ServerApp which accepts Port and Verbose 
	/// Arguments.
	/// </summary>
	/// <param name="port">Port on which Server will be Hosted</param>
	/// <param name="verbose"></param>
	ServerApp(int port = DEFAULT_PORT, bool verbose = false) {
		VERBOSE = verbose;
	}
};

/// <summary>
/// Method to Initialize and Run Server. Ideally should be called 
/// in a separate thread.
/// </summary>
/// <param name="server">ServerApp</param>
void runServer(ServerApp * server) {
	server = new ServerApp(8081, false);
	server->startServer();
}

/// <summary>
/// Method to Test Sockets package.
/// </summary>
/// <param name="argc">Argument Count</param>
/// <param name="argv">Arguments</param>
/// <returns></returns>
int main(int argc, char* argv[]) {
	ServerApp * server;
	std::thread serverThread(runServer, std::ref(server));
	
	std::string result;

	Client::Connect(result,"127.0.0.1", 8081, "Mom I'm on T.V. !!!", true);
	std::cout << "\n SERVER RESPONSE : " << result;
	Client::Connect(result, "127.0.0.1", 8081);

	serverThread.join();
	
	std::cout << "\n\n ";
}

#endif // TEST_SOCKETS
