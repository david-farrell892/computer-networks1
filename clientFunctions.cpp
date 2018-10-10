#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>
#include "clientFunctions.h"
#include <string>
#include <thread>
#include <iostream>

clientFunctions::clientFunctions()
{

}



int clientFunctions::createsocket()
{
	int opensocket = socket(AF_INET, SOCK_STREAM, 0); //open socket

	if (opensocket == -1) {
		std::cerr << "Error opening socket." << std::endl;
		return -1;
	}


	// clear the socket from the kernal so we don't get an error message if we try to run the server and connect to it again
	int yes = 1;
	if (setsockopt(opensocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		std::cerr << "Error clearing socket from kernal." << std::endl;
		return -1;
	}

	return opensocket;

}

void clientFunctions::runClient(int opensocket)
{
	char data[20000];
	vector<uint8_t> wire = request.endcode(); //build the http request from the url
	char ip[100];
	const char* hostname = request.site.c_str(); //covert site name (string) to char*

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
		return;
	}


	for (point = results; point != NULL; point = point->ai_next) //loop until we find something
	{
		h = (struct sockaddr_in *) point->ai_addr; //h is pointer to sockaddr stuctures stored in addrinfo struct 
		strcpy(ip, inet_ntoa(h->sin_addr)); //copy this IP contained in sockaddr that we found
	}

	freeaddrinfo(results); // free this struct




	
	struct sockaddr_in address; //structure containing the address details we are binding to
								

	address.sin_family = AF_INET; //now we set the parameters in that stucture
	address.sin_port = htons(request.port);
	address.sin_addr.s_addr = inet_addr(ip);

	if (connect(opensocket, (struct sockaddr *)&address, sizeof(address)) == -1) { //connect to the server
		std::cerr << "ERROR: Could not connect to server" << std::endl;
		close(opensocket);
		return;
	}



	if (send(opensocket, &wire[0], 1000, 0) == -1) { //send the message to server from the wire vector
		std::cerr << "ERROR: Could not send http message." << std::endl;
		close(opensocket);
		return;
	}

	std::cerr << "Client Sent Message" << std::endl;
	memset(&data, 0, sizeof(data)); // set the memory of the recieving buffer to 0

	if (recv(opensocket, data, 20000, 0) == -1) { //recieve message from server and store it in the data variable
		std::cerr << "ERROR: Could not recieve response message." << std::endl;
		close(opensocket);
		return;
	}

	std::cerr << "Client Recieved Message" << std::endl;

	request.consume(data);  //output this data to the console

}

