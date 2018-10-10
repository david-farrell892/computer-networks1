#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sstream>
#include "httpRequest.h"
#include "httpResponse.h"
#include "serverFunctions.h"
#include "thread.h"


thread::thread() {};

int thread::create_thread() {
	
};
