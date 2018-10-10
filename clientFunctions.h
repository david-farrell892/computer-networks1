#pragma once
#include <string>
#include <iostream>
#include "httpRequest.h"


using namespace std;

class clientFunctions {
public:
	clientFunctions();
	httpRequest request;
	int createsocket();
	void runClient(int opensocket);



};