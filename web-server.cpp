// The following code has been fully written by our group but has been done so by researching and applying elements from
// the example server code, Beej's guide to socket programming and various other helpful sources of information on the internet. 
// We've attempted to explain key lines of code using comments to show our level of understanding of the code. 

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sstream>
#include <pthread.h>
#include "httpResponse.h"
#include "serverFunctions.h"
#define max_queue_size 10 //global variables to set listening queue size, the maximum number of threads and max number of clients  
#define max_threads 10 //could use one variable to do all this to be honest but we'll have three to make it easier to read the code further on
#define max_clients 10 

void *handleConnection(void *id); //function we pass to pthread_create, each thread runs this function - tried putting this in serverFunctions.cpp to make it cleaner but it was just awkward to deal with 
static int clientConnection[max_clients]; //an array to hold all the client sockets 


int main(int argc, char *argv[])
{
	serverFunctions server; //initilise the server functions class
	pthread_t thread_id[max_threads]; //holds the thread ids

	
	server.set_args(argv); // Pass in the arguments sent during program execution, hostname, port, directory - see serverFunctions.cpp for more detials




	int opensocket = server.createsocket(server.port);  //open and bind the server socket - see serverFunctions.cpp for more detials
	if (opensocket == -1) {
		return 1;
	}

	std::cerr << std::endl;

	if (listen(opensocket, max_queue_size) == -1) { //tells the socket to start listening, max_queue_size is the maximum open connections
		return 1;
	}



	while (true) { //infinite while loop

		int num_thread = 0;  //thread counter

		while (num_thread < max_threads) { 

			struct sockaddr_in clientAddress;
			socklen_t clientAddressSize = sizeof(clientAddress);
			clientConnection[num_thread] = accept(opensocket, (struct sockaddr*) &clientAddress, &clientAddressSize); //accept a client connection

			int *id = (int*)malloc(sizeof(*id));
			*id = num_thread; //convert the int num_thread to a int pointer for pthread_create

			if (pthread_create(&thread_id[num_thread], NULL, handleConnection, id) < 0) { //create a thread and pass in the thread id number to the handleConnection function
				std::cerr << "Could not create thread." << std::endl;
			}

			num_thread++; //next thread 


		}
		
		for (int i = 0; i < max_threads; i++) {

			pthread_join(thread_id[i], NULL); //waits for each thread to terminate if they haven't already

		}

		std::cerr << "Maximum number of threads was reached, threads removed and now accepting new clients..." << std::endl;


	}

	return 0;
}


void *handleConnection(void *id) {
	
	httpResponse response; 
	char message[1000]; //buffer to hold the incoming message
	int thread_id = *(int *)id; //convert back to int

	
	memset(&message, 0, sizeof(message));
	if (recv(clientConnection[thread_id], message, 1000, 0) < 0) { //receive message from client and store it in the buffer
		std::cerr << "ERROR: Error recieving http Request";
		close(clientConnection[thread_id]);
		return NULL;
	}
	std::cerr << "Recieved a message from a Client, threadID: "<< thread_id << std::endl; 


	response.consume(message); //consume/parse the message - see httpResponse.cpp

	vector<uint8_t> responseMessage = response.endcode(); //generate the response - see httpResponse.cpp
	
	
	
	if (send(clientConnection[thread_id], &responseMessage[0], 20000, 0) == -1) { //send this response back to the client
		std::cerr << "ERROR: Could not send response. " << std::endl;
	}
	std::cerr << "Response sent" << std::endl;

	close(clientConnection[thread_id]); //close the connection to the client, job done. 
	
	std::cerr << std::endl;
	
	return NULL;
}