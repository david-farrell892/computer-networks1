#include "serverFunctions.h"
#include "httpResponse.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sstream>
#include <iostream>


serverFunctions::serverFunctions()
{

}


void serverFunctions::set_args(char * argv[]) //set the input arguments
{
	port = atoi(argv[2]);
	directory = argv[3];
	hostname = argv[1];


}

int serverFunctions::createsocket(int port)
{
	int opensocket = socket(AF_INET, SOCK_STREAM, 0); //(family, protocol type (UDP or TCP, TCP in this case), default protocol)

													  
	if (opensocket == -1) {
		std::cerr << "Error opening socket." << std::endl;
		return -1;
	}


	// clear the socket from the kernal so we don't get an error message if we try to run the server and connect to it again
	int yes = 1;
	if (setsockopt(opensocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		std::cerr << "Error clearing the socket from kernal" << std::endl;
		return -1;
	}

	struct addrinfo addr_details;
	struct addrinfo *results;
	struct addrinfo	*point;
	struct sockaddr_in *h;

	memset(&addr_details, 0, sizeof(addr_details));
	addr_details.ai_family = AF_INET;
	addr_details.ai_socktype = SOCK_STREAM;

	int addr = getaddrinfo(hostname, "http", &addr_details, &results); //4 arguments - hostname, service name(http, ftp, smtp etc.), addrinfo struct with some details filled out, linked list of struct addrinfos 
	if (addr == -1)
	{
		std::cerr << "Error getting address info. " << std::endl;
		return -1;
	}


	for (point = results; point != NULL; point = point->ai_next) //loop until we find something
	{
		h = (struct sockaddr_in *) point->ai_addr; //h is pointer to sockaddr stuctures stored in addrinfo struct 
		strcpy(ip, inet_ntoa(h->sin_addr)); //copy this IP contained in sockaddr that we found
	}

	freeaddrinfo(results); // free this struct



	

	struct sockaddr_in address; //structure containing the address details we are binding to
								//now we set the parameters in that stucture

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = inet_addr(ip);

	int bindsocket = bind(opensocket, (struct sockaddr*) &address, sizeof(address)); //binds the listening socket to a port
	if (bindsocket == -1) {
		std::cerr << "Error binding socket." << std::endl;
		return -1;
	}

	std::cerr << "Socket opened and binded." << std::endl; //output success

	return opensocket;

}







