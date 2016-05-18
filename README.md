# prime_client_server

Client/server application to test the primality of an unsigned integer. The client issues a request to the server which will return a boolean value.

==Quick Setup==

=Client=

g++ -Wall client.cpp -o client -std=c++11 -L/usr/local/lib -lzmq

Usage: client [OPTION...]
-s <server> ipv4 address of server, or localhost (default = localhost)
-p <port> server port (default = 5555)
-n <number> number to test

=Server=

g++ -Wall server.cpp -o server -std=c++11 -L/usr/local/lib -lzmq -lpthread

Usage: server [OPTION...]
	-p <port> port (default = 5555)
	-t <threads> number of worker threads

Both require the ZeroMQ library available at http://zeromq.org/

Typically the library will be installed in /usr/local/lib.


==Notes==

The project is functional but unit testing is incomplete. I just didn't have enough free time available. Sorry.
The client/server "determine if prime" functionality is all there (if a little bare bones).
I used the ZeroMQ destributed messaging library because it's just about the coolest thing ever.
It's overkill here but this was a good excuse to play with it and I'd use it again.

With regards to unit testing and coverage I've attempted to show the test approach I would take with more time.

prime_test.cpp uses the Google Test framework to do some simple unit tests on the PrimeCandidate class defined in prime.hpp

Compile with something similar to:

g++ -Wall prime_test.cpp -o prime_test -std=c++11 -L/usr/local/lib -lgtest -lpthread -I/path/to/googletest/include/

mock-client.cpp and request_reply_interface.hpp are an attempt to create a mock using Google Mock for the client's interface to the server. Needs work.

Compile with something similar to:

g++ -Wall mock-client.cpp -o mock_client -L/usr/local/lib -lgmock -lzmq -lpthread -I/path/to/googlemock/include/ -I/path/to/googletest/include/
