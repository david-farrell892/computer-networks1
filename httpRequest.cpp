#include "httpRequest.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;


httpRequest::httpRequest() {



}


vector<uint8_t> httpRequest::endcode()
{
	path = url;
	int http_end = path.find('/', 0);
	path.erase(0, (http_end + 2)); //erase http://

	int site_end = path.find(':', 0); //find first instance of :
	site = path.substr(0, site_end); //site is everything up to that point i.e. "localhost"
	std::cerr << site << std::endl;
	path.erase(0, site_end + 1); //erase up to :
	int port_end = path.find('/', 0); //find first instance of /

	string port_str = path.substr(0, port_end); //port is everything up to that port i.e. 4000
	stringstream ss(port_str); //covert this to int
	ss >> port;

	path.erase(0, port_end + 1); //erase up to /

	string full = "GET " + path + " HTTP/1.0" + "\r\n\r\n"; //build the http request
	std::cerr << "Sending HTTP Request: " << full << std::endl;  //optional for debugging
	vector<uint8_t> full_vec(full.begin(), full.end()); //convert this string to a vector for to send
	full_vec.push_back('\0');
	return full_vec;
}

void httpRequest::consume(const char* wire)
{

	string version;
	string code;
	string fileText;
	int code_num;
	const char* front = wire; //pointer to front of wire
	const char* back = wire;

	while (*back != ' ') { //while we haven't reached the end of HTTP/1.0

		back++;
	}
	version = string(front, back);

	back++;
	front = back; //move front to start of next check

	while (*back != ' ') { //while we haven't reach the end of response code (200, 400, 404)
		back++;
	}

	code = string(front, back);
	stringstream ss(code); //convert code to int
	ss >> code_num;

	if (code_num == 200) {

		for (int i = 0; i < 4; i++) { //to get past the "OK"
			back++;
		}
		code = string(front, back);
		front = back;
		std::cerr << "Response: " << version << " " << code << std::endl;

		while (*back != '\0') { //while we haven't reach the end of file text
			back++;
		}

		fileText = string(front, back);

		if (path.rfind("/", 0) == 0) { //remove the / from the front of the file name if there is one
			path.erase(0, 1);
		}


		ofstream file(path); //create file, path holds the file name from the url we parsed earlier
		if (file.is_open()) {
			file << fileText;

			file.close();
		}
		else {
			std::cerr << "Error creating file in this directory." << std::endl;
		}
		std::cerr << "The requested file has been added to your directory." << std::endl;
	}
	else {
		string response(wire);
		std::cerr << "Response: " <<  response << std::endl; //output response 
	}

}
