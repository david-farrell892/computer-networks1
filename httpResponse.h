#include <iostream>
#include <string>
#include <vector>
using namespace std;

class httpResponse {
public:

	httpResponse();
	string versionCheck = "HTTP/1.0";
	string methodCheck = "GET";
	string fileData; //holds the file data
	string file; //holds the file name
	string statusCode;
	vector<uint8_t> endcode();
	void consume(const char * wire);
	void searchFile();
};