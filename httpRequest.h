#include <iostream>
#include <string>
#include<vector>
using namespace std;

class httpRequest {
public:
	httpRequest();
	string url;
	string site;
	string path;
	int port;
	vector<uint8_t> endcode();
	void consume(const char* wire);
};