/*
*	Client.cpp
*	Connect with the prime server, send request, await reply.
*/

#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include "request_reply_interface.hpp"

static const char *DEFAULT_SERVER = "localhost";
static const unsigned short DEFAULT_PORT = 5555;
static const unsigned int DEFAULT_NUMBER = (unsigned int) 2147483647; // 2^32 - 1

const char *g_conf_server;
unsigned short g_conf_port;
unsigned int g_conf_number;

class IsPrimeInterface : public RequestReplyInterface
{
public:
	bool Init(void)
	{
		context = new zmq::context_t(1);
		if (!context)
			return false;

		socket = new zmq::socket_t(*context, ZMQ_REQ);
		if (!socket)
			return false;

		return true;
	}

	bool Connect(const char *server, unsigned short port)
	{
		char connect_str[20];
		if (std::strlen(server) > 20)
			return false;

		sprintf(connect_str, "tcp://%s:%d", g_conf_server, g_conf_port);
		std::cout << "Connecting to " << connect_str << std::endl;
    	socket->connect(connect_str);

		return true;
	}

	bool Send(unsigned int number)
	{
		zmq::message_t msg(sizeof(int));
		memcpy(msg.data(), &number, sizeof(int));

		std::cout << "Testing the number: " << number << std::endl;
		if (socket->send(msg) == -1)
			return false;

		return true;
	}

	bool GetReply()
	{
		zmq::message_t reply;
		socket->recv(&reply);
		return *(static_cast<int *>(reply.data())) ? true : false;
	}

private:
    zmq::context_t *context;
    zmq::socket_t *socket;
};

static void 
usage(void)
{
	std::cout << "Usage: client [OPTION...]" << std::endl;
	std::cout << "  -s <server> ipv4 address of server, or localhost (default = " << DEFAULT_SERVER << ")" << std::endl;
	std::cout << "  -p <port> server port (default = " << DEFAULT_PORT << ")" << std::endl;
	std::cout << "  -n <number> number to test" << std::endl;
}

static bool
initialize(int argc, char *argv[])
{
	int c;

	g_conf_server = DEFAULT_SERVER;
	g_conf_port = DEFAULT_PORT;
	g_conf_number = DEFAULT_NUMBER;

	while ((c = getopt(argc, argv, "s:p:n:")) != -1)
	{
		switch (c)
		{
		case 's':
			g_conf_server = optarg;
			break;
		case 'p':
			g_conf_port = std::strtoul(optarg, NULL, 0);
			break;
		case 'n':
			g_conf_number = std::strtoul(optarg, NULL, 0);
			break;
		default:
			usage();
			return false;
		}
	}
	return true;
}

int main(int argc, char *argv[])
{
	if (!initialize(argc, argv))
		exit(0);
	
	IsPrimeInterface *pInterface = new IsPrimeInterface;

	if (!pInterface->Init() || !pInterface->Connect(g_conf_server, g_conf_port) || !pInterface->Send(g_conf_number))
		return 0;

	if (pInterface->GetReply())
		std::cout << "Result: prime" << std::endl;
	else
		std::cout << "Result: not prime" << std::endl;

    return 1;
}
