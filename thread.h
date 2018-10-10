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


class thread{
public:
	thread();
	int create_thread();
};