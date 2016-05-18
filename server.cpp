/*
    Server.cpp
	Determine the primacy of an unsigned int.
	Based on ZMQ's mtserver example.
*/

#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <cassert>
#include <string>
#include <iostream>
#include <zmq.hpp>
#include "prime.hpp"

static const uint16_t DEFAULT_PORT = 5555;
static const uint8_t DEFAULT_THREADS = 5;

unsigned short g_conf_port;
unsigned int g_conf_threads;

static void 
usage(void)
{
	std::cout << "Usage: server [OPTION...]" << std::endl;
	std::cout << "  -p <port> port (default = " << DEFAULT_PORT << ")" << std::endl;
	std::cout << "  -t <threads> number of worker threads" << std::endl;
}

static bool
init(int argc, char *argv[])
{
	//TODO Sanity test port and num threads

	int c;

	g_conf_port = DEFAULT_PORT;
	g_conf_threads = DEFAULT_THREADS;

	while ((c = getopt(argc, argv, "p:t:")) != -1)
	{
		switch (c)
		{
		case 'p':
			g_conf_port = std::strtoul(optarg, NULL, 0);
			break;
		case 't':
			g_conf_threads = std::strtoul(optarg, NULL, 0);
			break;
		default:
			usage();
			return false;
		}
	}

	return true;
}

void *worker_thread(void *arg)
{
    zmq::context_t *context = (zmq::context_t *)arg;
    zmq::socket_t socket (*context, ZMQ_REP);
    socket.connect("inproc://workers");
	
	for (;;)
	{
        // Wait for a request
        zmq::message_t request;
        socket.recv(&request);
		
		uint32_t value = *(static_cast<uint32_t *>(request.data()));
        std::cout << "Received request: [" << value << "]" << std::endl;

		// Determine primacy
		PrimeCandidate candidate(value);

		int result = 0;
		if (candidate.IsPrime())
			result = 1;

        // Reply
        zmq::message_t reply(sizeof(int));
        memcpy((void *)reply.data(), &result, sizeof(int));
        socket.send(reply);
    }
    return (NULL);
}

int main(int argc, char *argv[])
{
	if (!init(argc, argv))
		exit(0);

	std::cout << "Starting prime server with " << g_conf_threads << " worker thread(s)" << std::endl;
	std::cout << "Listening on port " << g_conf_port << std::endl;

    // Prepare ZMQ context and sockets
    zmq::context_t context(1);
    zmq::socket_t clients (context, ZMQ_ROUTER);
	char bind_str[20];
	sprintf(bind_str, "tcp://*:%d", g_conf_port);
    clients.bind (bind_str);
    zmq::socket_t workers (context, ZMQ_DEALER);
    workers.bind ("inproc://workers");


    // Launch pool of worker threads
    for (uint8_t thread_nbr = 0; thread_nbr != g_conf_threads; thread_nbr++) 
	{
        pthread_t worker;
        pthread_create(&worker, NULL, worker_thread, (void *) &context);
    }

    // Connect work threads to client threads via a queue
	zmq::proxy(static_cast<void *>(clients), static_cast<void *>(workers), NULL);

    return 0;
}
    
