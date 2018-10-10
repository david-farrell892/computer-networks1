#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sstream>
#include "clientFunctions.h"
#include <string>
#include <thread>
#include <iostream>



int main(int argc, char *argv[])
{

	clientFunctions client;
	int count = 1; //counter to keep track of how many input arguments we have dealt with

	if (argc < 2) { //make sure there is a url to decode first
		std::cerr << "Please include a url and try again." << std::endl;
	}

	while (count < argc) {


		client.request.url = argv[count]; //set the url in httpRequest


		int opensocket = client.createsocket(); //open socket
		if (opensocket == -1) {
			return 1;
		}

		client.runClient(opensocket); //do everything


		count++; 

		close(opensocket); //close socket
	}


	return 0; //no more urls, exit


}
