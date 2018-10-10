#pragma once
#include <string>
#include <iostream>



using namespace std;




class serverFunctions {
public:
	serverFunctions();
	const char* hostname;
	int port;
	string directory;
	char ip[100];
	void set_args(char *argv[]);
	int createsocket(int port);


};