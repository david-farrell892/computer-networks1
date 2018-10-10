#include "httpResponse.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
httpResponse::httpResponse()
{

}


vector<uint8_t> httpResponse::endcode()
{
	
	string full_response = "HTTP/1.0 " + statusCode + " " + fileData + "\r\n\r\n"; //generate the reponse
	//std::cerr << "Sending to Client: " << full_response << std::endl; 
	
	vector<uint8_t> response(full_response.begin(), full_response.end()); //convert response to vector
	response.push_back('\0');

	return response;

}

void httpResponse::consume(const char* wire)
{

	string method; 
	string version;
	
	const char* front = wire; //pointer to front of wire
	const char* back = wire; 

	while (*back != ' ') { //while we haven't reached the end of GET

		back++;
	}

	method = string(front, back); 
	std::cerr << method << " ";
	if (method != methodCheck) { //compare to see if this is what we expected 
		
		statusCode = "400 Bad Request";

		return;
	}

	back++;
	front = back; //move front to start of next check
	while (*back != ' ') { //while we haven't reach the end of the file name
		back++;
	}

	file = string(front, back);
	std::cerr << file << " ";
	back++;
	
	front = back;
	while (*back != '\r') {//while we haven't reached the end of http/1.0 
		
		back++;
	}

	version = string(front, back);
	std::cerr << version << std::endl;
	
	if (version != "HTTP/1.0") {//check to see if that looks right

		statusCode = "400 Bad Request";
		return;
	}

	
	
	searchFile();
}

void httpResponse::searchFile()
{
	if (file.rfind("/", 0) == 0) { //remove the / from the front of the file name if there is one
		file.erase(0, 1);
	}
	ifstream search(file); //contruct ifstream to read in from file
	 
	if (search.is_open()) { //if it opened that means it exists

		getline(search, fileData, '\0'); //read in from file and stick it in fileData until we see a '\0' (null terminator)
		search.close(); 
		statusCode = "200 OK";
		
		std::cerr << "File found" << std::endl;
	}
	else { //file doesn't exist
		std::cerr << "File not found" << std::endl;
		statusCode = "404 Not Found";
	}

}
